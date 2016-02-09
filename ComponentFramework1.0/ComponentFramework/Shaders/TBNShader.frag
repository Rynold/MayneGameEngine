#version 430 core

//in vec3 Color;
struct Material {
	sampler2D baseTex;
	sampler2D specular;
	sampler2D normalMap;
	sampler2D roughnessMap;
	float roughness;
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

out vec4 fColor;

uniform sampler2D shadowMap;
uniform Material material;
uniform vec3 viewPos;

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 1  
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcDirLightBlinn(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLightBlinn(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float shadow;
vec3 tangentLightPos;
vec3 tangentViewPos;
vec3 tangentFragPos;

float ShadowCalculation(vec4 fpls);

void main()
{
	tangentFragPos = TBN * FragPos;
	tangentViewPos = TBN * viewPos;

    //vec3 norm = normalize(Normal);
	vec3 norm = texture(material.normalMap, texCoord).rgb;
	norm = normalize(norm * 2.0 - 1.0);

    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);

    vec3 result = CalcDirLightBlinn(dirLight, norm, viewDir);

    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //    result += CalcPointLightBlinn(pointLights[i], norm, FragPos, viewDir);

    fColor = vec4(result, 1.0);
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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.baseTex, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.baseTex, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcDirLightBlinn(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 fragColor = texture(material.baseTex, texCoord).rgb;

	//Ambient
	vec3 ambient = 0.15 * fragColor;

	tangentLightPos = TBN * dirLight.position;

    //Diffuse
	vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.color * diff * fragColor;

	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.roughness);
	vec3 specular = light.color * spec;// * texture(material.specular, texCoord).rgb;

	//Calculate Shadows
	shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);

	return (ambient + (1.0 - shadow) * (diffuse + specular));
	//return ambient + diffuse + specular;
} 

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.roughness);
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.baseTex, texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.baseTex, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
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