#include "pointVector.h"

using namespace Gdiplus;

bool operator==(POINT p1, POINT p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y);
}

bool operator!=(POINT p1, POINT p2)
{
	return (p1.x != p2.x) || (p1.y != p2.y);
}

POINT operator+(POINT p1, POINT p2)
{
	return POINT({ p1.x + p2.x , p1.y + p2.y });
}

POINT operator-(POINT p1, POINT p2)
{
	return POINT({ p1.x - p2.x , p1.y - p2.y });
}

POINT operator*(POINT p1, POINT p2)
{
	return POINT({ p1.x * p2.x , p1.y * p2.y });
}
