#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "MColor.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "glm.hpp"

class Light : public GameObject
{
public:
	Light()
	{
		color = MColor::GREEN();
	}
	virtual ~Light()
	{
		glDeleteBuffers(1, &depthMapFBO);
	}

	MColor color;

	GLuint depthMap;
	GLuint depthMapFBO;

	GLuint SHADOW_WIDTH;
	GLuint SHADOW_HEIGHT;

	const char* shaderID;

	virtual void GenerateDepthMapFBO(){};
	virtual void CreateDepthMap(vector<GameObject*> GameObjects, glm::mat4 lightSpaceMatrix){};
	
};

class DirectionalLight : public Light
{
public:
	DirectionalLight()
	{
		_direction = Vec3(0.0, -1.0, 0.0);
		SHADOW_HEIGHT = 1024;
		SHADOW_WIDTH = 1024;

		shaderID = ShaderManager::GetInstance()->Insert("Triangles", std::shared_ptr<Shader>(new Shader("triangles.vert", "triangles.frag")));
	}
	virtual ~DirectionalLight()
	{
	}

	Vec3 _direction;

	virtual std::shared_ptr<Shader> GetShader() override
	{
		return ShaderManager::GetInstance()->Contains(shaderID);
	}

	virtual void CreateDepthMap(vector<GameObject*> GameObjects, glm::mat4 lightSpaceMatrix) override
	{
		std::shared_ptr<Shader> shader = ShaderManager::GetInstance()->Contains(shaderID);
		if (shader)
		{
			shader->Use();
			glUniformMatrix4fv(glGetUniformLocation(shader->Program, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);

			//Render each object with the shadow shader for the depth
			for each(GameObject* object in GameObjects)
			{
				object->Draw(shader);// , depthMap, 0, object->reflective);
			}
		
		}
		shader = nullptr;
	}

	virtual void GenerateDepthMapFBO() override
	{
		//create our own frame buffer to use for shadows
		glGenFramebuffers(1, &depthMapFBO);

		//Generate the texture used for shadows
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	}
};

class PointLight : public Light
{
public:
	PointLight()
	{
		constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032f;
	}
	virtual ~PointLight()
	{
	}

	float constant;
	float linear;
	float quadratic;
};

#endif