#pragma once
#include "headers.h"

void slashAtk(GameObject* obj, int dmg);

class Player : public Actor
{
private:
	TIMER dodgeTimer;
	bool dodge;
public:
	Player(POINT p);

	virtual void draw(HDC& hdc);

	bool isDodge() { return dodge; }
	void setDodge() { dodge = true; }
	void unsetDodge() { dodge = false; }

	bool canDodge() { return dodgeTimer.getElapsedTime() > 600; }
	void setDodgeTimer() { dodgeTimer.set(); }

};