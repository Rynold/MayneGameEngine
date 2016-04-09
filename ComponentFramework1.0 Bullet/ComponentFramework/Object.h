#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
class Object
{
public:
	Object();
	~Object();

	Transform* _transform;
};

#endif;