#pragma once
#ifndef __PVECTOR__
#define __PVECTOR__

#include "framework.h"

bool operator==(POINT p1, POINT p2);

bool operator!=(POINT p1, POINT p2);

POINT operator+(POINT p1, POINT p2);

POINT operator-(POINT p1, POINT p2);

POINT operator*(POINT p1, POINT p2);

template<typename T>
POINT operator*(T x, POINT p2)
{
	return POINT({ long(x * p2.x) , long(x * p2.y) });
}

template<typename T>
POINT operator/(POINT p, T x)
{
	return POINT({ long(p.x / x), long(p.y / x) });
}

template<typename T>
POINT operator/(T x, POINT p)
{
	return POINT({ long(p.x / x), long(p.y / x) });
}

#endif