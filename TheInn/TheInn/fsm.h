#pragma once
#include "headers.h"

class StateMachine
{
public:
	virtual void setState(GameObject* obj, int input) = 0;
	virtual void update(GameObject* obj);
	virtual void enter(GameObject* obj) {}
	virtual void exit(GameObject* obj) {}
};

class PlayerIDLE
{
public:
	virtual void setState(GameObject* obj, int input)
	{
		Actor* test = dynamic_cast<Actor*>(obj);
	}
};