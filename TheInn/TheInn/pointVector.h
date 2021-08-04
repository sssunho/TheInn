#pragma once
#ifndef __PVECTOR__
#define __PVECTOR__

#include "framework.h"

bool operator==(POINT p1, POINT p2);

POINT operator+(POINT p1, POINT p2);

POINT operator-(POINT p1, POINT p2);

POINT operator*(POINT p1, POINT p2);

template<typename T>
POINT operator*(T x, POINT p2)
{
	return POINT({ long(x*p2.x) , long(x*p2.y) });
}
#endif