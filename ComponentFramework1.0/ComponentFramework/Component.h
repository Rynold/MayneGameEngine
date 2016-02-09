#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include "Transform.h"

class Component
{
public:
	Component() {};
	~Component() {};

	Transform _transform;
};

#endif