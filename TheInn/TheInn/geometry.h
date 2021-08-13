#pragma once
#include "VECTOR.h"

class Area
{
public:
	virtual bool isIn(VECTOR p) = 0;
};

class Circle : public Area
{
public:
	Circle(VECTOR c, float r) : c(c), r(r) {}
	VECTOR c;
	float r;

	virtual bool isIn(VECTOR p)
	{
		return (p - c).getScalar() < r;
	}
};

class Sector : public Circle
{
public:
	Sector(VECTOR c, float r, float dir = 0, float rng = 0) : 
		Circle(c, r), direction(dir), range(rng) {}

	float direction;
	float range;

	virtual bool isIn(VECTOR p)
	{
		return Circle::isIn(p) &&
			((p - c) * VECTOR(direction)) / ((p - c).getScalar() * VECTOR(direction).getScalar())
			>= cos(range / 2);
	}
};

class LINE
{

public:
	VECTOR v1;
	VECTOR v2;

	LINE() : v1(), v2() {};
	LINE(VECTOR _v1, VECTOR _v2) : v1(_v1), v2(_v2) {};
	LINE(double r) : v1(), v2(r) {};

	VECTOR getUnit() { return (v1 - v2).getUnit(); }
	double getScalar() { return (v1 - v2).getScalar(); }
	double rotate(double r) { v1 = v1.rotate(r); v2 = v2.rotate(r); }
	friend bool getCrossPoint(VECTOR* v, LINE l1, LINE l2);
	bool onLine(VECTOR v);
	VECTOR getPointProj(VECTOR v);
	double getT(VECTOR v);

};
