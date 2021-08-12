#pragma once
#include "headers.h"
class Player : public Actor
{
private:
	bool controllable;

public:
	Player(POINT p) : Actor(p, { 0, 0 }, "swordman", DIRECTION::D, 2) {}

};