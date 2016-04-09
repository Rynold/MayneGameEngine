#pragma once
#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "MMath.h"

class MathUtil
{
public:
	inline static float DegToRads(float deg)
	{
		return deg * DEGREES_TO_RADIANS;
	}
};

#endif