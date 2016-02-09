#pragma once
#ifndef MCOLOR_H
#define MCOLOR_H

class MColor
{
public:
	MColor()
	{
		R = 1.0f;
		G = 1.0f;
		B = 1.0f;
		A = 1.0f;
	}
	MColor(float r_, float g_, float b_, float a_)
	{
		R = r_;
		G = g_;
		B = b_;
		A = a_;
	}

	~MColor() {}

	float R;
	float G;
	float B;
	float A;

	inline static MColor RED(){ return MColor(1.0f, 0.0f, 0.0f, 1.0f); }

	inline static MColor GREEN(){ return MColor(0.0f, 1.0f, 0.0f, 1.0f); }

	inline static MColor BLUE(){ return MColor(0.0f, 0.0f, 1.0f, 1.0f); }

	inline static MColor WHITE() { return MColor(1.0f, 1.0f, 1.0f, 1.0f); }

};

#endif