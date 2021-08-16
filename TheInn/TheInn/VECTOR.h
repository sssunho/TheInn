#ifndef __VECTOR__
#define __VECTOR__

#include "framework.h"
#include <cmath>

const float PI = atanf(1) * 4;

class VECTOR
{
public:
	float x;
	float y;

	VECTOR() : x(0), y(0) {};
	VECTOR(float _e1, float _e2) : x(_e1), y(_e2) {};
	VECTOR(float r);
	VECTOR(POINT p) { x = ::round(p.x); y = ::round(p.y); }

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
	float operator*(VECTOR v)
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
		return POINT({ (long)::round(x), (long)::round(y) });
	}

	float getScalar()
	{
		return sqrt(x*x + y * y);
	}

	VECTOR getUnit()
	{
		return (1 / getScalar())*(*this);
	}

	VECTOR rotate(double r)
	{
		return { cosf(r)*x - sinf(r)*y, sinf(r)*x + cosf(r)*y };
	}

	VECTOR round()
	{
		return { ::round(x), ::round(y) };
	}

	float getRad()
	{
		if (getScalar() == 0.0f)
			return 0.0f;
		return (y < 0.0f ? -1.0f : 1.0f) * acosf(x / getScalar());
	}
};

#endif
