#pragma once
#include "framework.h"
#include "headers.h"


class Enemy : public Actor
{
private:
	float detectRange;
	int damage;

public:
	Enemy(POINT p, POINT v, const string spriteName, DIRECTION dir, int size);
};
