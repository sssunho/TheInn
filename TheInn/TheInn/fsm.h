#pragma once
#include "headers.h"

class StateMachine
{
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine * state) = 0;
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj) {}
	virtual void exit(GameObject* obj) {}
};

class PlayerIDLE : public StateMachine
{
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class PlayerMove : public StateMachine
{
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class Attack : public StateMachine
{
protected :
	TIMER timer;
	bool pretime = true;
	bool hittime = true;
};

class PlayerCombo1 : public Attack
{
private:
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class PlayerCombo2 : public Attack
{

public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class PlayerCombo3 : public Attack
{

public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class PlayerHIT : public StateMachine
{
protected:
	TIMER timer;

public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class HIT : public StateMachine
{
	TIMER timer;
	string oldSprite;
	int damage;
public:
	HIT(int dmg = 0) : damage(dmg) {};
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class EnemyIDLE : public StateMachine
{
private:
	TIMER timer;

public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};