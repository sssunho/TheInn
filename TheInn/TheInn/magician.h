#pragma once
#include "headers.h"

class Magician : public Enemy
{
private:
	TIMER timer;

public:
	Animation chargeAni;
	Magician(VECTOR p);
	Magician(VECTOR p, DIRECTION d, float range);
	virtual void update(float dt);
	virtual void draw(HDC& hdc);
};

class MagicianIDLE : public StateMachine
{
private:
	TIMER timer;
	TIMER patrolTimer;

public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class MagicianOnMove : public StateMachine
{
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class MagicianPatrol : public MagicianOnMove
{
	TIMER patrolTimer;
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
};

class MagicianHIT : public StateMachine
{
private:
	int damage;
	TIMER timer;

public:
	MagicianHIT(int damage = 10) : damage(damage) {}
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class MagicianyKnockBack : public StateMachine
{
private:
	TIMER timer;

public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);

};

class MagicianAtk : public StateMachine
{
private:
	TIMER timer;
	bool posttime;

public:
	MagicianAtk() : timer(), posttime(false) {}
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class MagicianDead : public StateMachine
{
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};