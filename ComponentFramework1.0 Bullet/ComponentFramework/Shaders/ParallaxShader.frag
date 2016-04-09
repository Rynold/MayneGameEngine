#version 430 core
//#extension GL_EXT_gpu_shader4 : enable

uint baseFlag   = 0x00000001;
uint specFlag   = 0x00000010;
uint normFlag   = 0x00000100;
uint roughFlag  = 0x00001000;
uint heightFlag = 0x00010000;
uint reflFlag   = 0x00100000;

//in vec3 Color;
struct Material {
	sampler2D baseTex;
	sampler2D specular;
	sampler2D normalMap;
	sampler2D roughnessMap;
	sampler2D depthMap;
	float roughness;
	uint matFlags;
};

struct DirLight
{
	vec3 position;
	vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	vec3 color;
};

struct PointLight
{
	vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	vec3 color;
};

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in mat3 TBN;
in vec3 Position;

out vec4 fColor;

uniform sampler2D shadowMap;
uniform Material material;
uniform vec3 viewPos;
uniform float height_scale;
uniform int reflective;
uniform samplerCube skybox;

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 1  
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcDirLightBlinn(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLightBlinn(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float shadow;
// As an optimisation these variables should be calculated in the vertex shader.
// The vertex shader runs less often then the frag shader and these values would not
// change until we get to the next vertex.
vec3 tangentLightPos;
vec3 tangentViewPos;
vec3 tangentFragPos;

vec2 parTexCoords;

float ShadowCalculation(vec4 fpls);

//This function will return the displaced texCoords to use when sampling from our textures.
vec2 ParallaxMapping(vec2 texC, vec3 viewD);

void main()
{
	tangentFragPos = TBN * FragPos;
	tangentViewPos = TBN * viewPos;

	vec3 viewDir = normalize(tangentViewPos - tangentFragPos);

	parTexCoords = ParallaxMapping(texCoord, viewDir);
	
	
	//vec3 norm = normalize(Normal);
	vec3 norm = texture(material.normalMap, parTexCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);

	vec3 result = CalcDirLightBlinn(dirLight, norm, viewDir);
	
	//for(int i = 0; i < NR_POINT_LIGHTS; i++)
	//    result += CalcPointLightBlinn(pointLights[i], norm, FragPos, viewDir);
	fColor = vec4(result, 1.0);
}

vec2 ParallaxMapping(vec2 texC, vec3 viewD)
{
	//The minimum and maximum number of layers to sample.
	// While looking straight at the surface you will need less layers because there isn't as much
	// texture displacement happening. By taking less samples while looking straight on a surface but
	// more samples while looking at the surface from an angle you only sample the necessary amount.
	const float minLayers = 10;
	const float maxLayers = 20;
	
	// By getting the dot product of viewD and the positive z direction we can get the angle we are looking
	// from to align the amount of samples we need.
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0,0.0,1.0), viewD)));
	
	// Calculate the size of each layer
	float layerDepth = 1.0 / numLayers;
	// Depth of the current layer
	float currentLayerDepth = 0.0;
	
	// Dividing the viewDir's xy by its z we are essentially giving us a much larger P vector
	// because the viewD is normalized, therefor the z component will be between 0.0 and 1.0.
	// when viewD is largly parralel to the surface its z component is close to 0.0, which will
	// return an even larger z value.
	vec2 p = viewD.xy / viewD.z * 0.1;
	
	// The amount to shift the texture coord per layer
	vec2 deltaTexCoords = p / numLayers;
	
	// Get the inital value
	vec2 currentTexCoords = texC;
	float currentDepthMapValue = texture(material.depthMap, currentTexCoords).r;
	
	// Iterate through all the depth layers until we find a depthMap value less than the layer's depth value.
	while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(material.depthMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
	// Get the texture coordinates before collision
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(material.depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
	 
	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	
    return finalTexCoords;
}

float ShadowCalculation(vec4 fpls, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fpls.xyz / fpls.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Check whether current frag pos is in shadow
	// The bias makes sure that some frags are not incorrectly thought to be under the surface.
	// By multiplying the bias by the angle of the frag we can make a better assumption of what it is.
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float Shadow = 0;

	// We multisample the serrounding texels and average the results.
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			Shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	Shadow /= 9.0;

	// If the frag is outside of the projections view don't cast any shadow.
	if(projCoords.z > 1.0)
		Shadow = 0.0;

    return Shadow;
}

vec3 CalcDirLightBlinn(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 fragColor = texture(material.baseTex, parTexCoords).rgb;

	//Ambient
	vec3 ambient = 0.15 * fragColor;

	tangentLightPos = TBN * dirLight.position;
	
    //Diffuse
	vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.color * diff * fragColor;

	//Specular
	//vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.roughness);
	vec3 specular;
	if((material.matFlags & specFlag) != 0)
	{
		specular = light.color * spec * texture(material.specular, parTexCoords).rgb;
	}
	else{
		specular = light.color * spec;
	}

	//Calculate Shadows
	shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);

	return (ambient + (1.0 - shadow) * (diffuse + specular));
	//return ambient + diffuse + specular;
} 

vec3 CalcPointLightBlinn(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 fragColor = texture(material.baseTex, texCoord).rgb;

	vec3 ambient = 0.05 * fragColor;

    vec3 lightDir = normalize(light.position - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

    // Diffuse shading
    float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.color * diff * fragColor;

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.roughness);
    vec3 specular = light.color * spec * texture(material.specular, texCoord).rgb;
	
	// Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // Combine results
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + (1.0 - shadow) *  (diffuse + specular));
	}