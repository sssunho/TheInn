#pragma once
#include "headers.h"

class Projectile : public Actor
{
private:
	int timeLimit;
	TIMER timer;

public:
	Projectile(POINT p = { 0, 0 },
		POINT v = { 0, 0 },
		const string spriteName = NULL,
		int timeLimit = 8000);

	virtual void update(float dt);
};