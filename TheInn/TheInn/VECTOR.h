#ifndef __VECTOR__
#define __VECTOR__

#include "framework.h"
#include <cmath>

const float PI = atan(1) * 4;

class VECTOR
{
public:
	float x;
	float y;

	VECTOR() : x(0), y(0) {};
	VECTOR(float _e1, float _e2) : x(_e1), y(_e2) {};
	VECTOR(float r);

	VECTOR operator+(VECTOR v)
	{
		VECTOR temp = { x + v.x, y + v.y };
		return temp;
	}
	VECTOR operator-(VECTOR v)
	{
		VECTOR temp = { x - v.x, y - v.y };
		return temp;
	}
	double operator*(VECTOR v)
	{
		return x*v.x + y*v.y;
	}
	template<typename T>
	friend VECTOR operator*(T k, VECTOR v)
	{
		VECTOR temp{ k*v.x, k*v.y };
		return temp;
	}

	operator const POINT() const
	{
		return POINT({ (long)round(x), (long)round(y) });
	}
};

#endif
