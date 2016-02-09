#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "MColor.h"
#include "GameObject.h"

class Light : public GameObject
{
public:
	Light()
	{
		color = MColor::GREEN();
	}
	~Light()
	{}

	MColor color;

};

class DirectionalLight : public Light
{
public:
	DirectionalLight()
	{
		_direction = Vec3(0.0, -1.0, 0.0);
	}
	~DirectionalLight()
	{
	}

	Vec3 _direction;
};

class PointLight : public Light
{
public:
	PointLight()
	{
		constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032;
	}
	~PointLight()
	{
	}

	float constant;
	float linear;
	float quadratic;
};

#endif