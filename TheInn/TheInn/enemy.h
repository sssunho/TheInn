#pragma once

#include "framework.h"
#include "gameObject.h"
#include "fsm.h"
#include <stack>

using std::stack;

class Enemy : public Actor
{
	friend class EnemyChase;

private:
	static int temp;
	string enemyName;
	Area* detectRange;
	Area* atkRange;
	Area* chaseRange;
	stack<POINT> path;
	POINT dest;
	float speed;
	int damage;

public:

	virtual void draw(HDC& hdc);
	virtual void update(float dt);
	virtual void setDirection(DIRECTION d);

	void setDetectRange(Area* area)
	{
		if (detectRange)
			delete detectRange;
		detectRange = area;
	}
	void setAtkRange(Area* area)
	{
		if (atkRange)
			delete atkRange;
		atkRange = area;

	}
	void setChaseRange(Area* area)
	{
		if (chaseRange)
			delete chaseRange;
		chaseRange = area;
	}

	void findPath(VECTOR dest);

	bool isInAtkArea(VECTOR v) 
	{ 
		if (atkRange) 
			return atkRange->isIn(v); 
		return false; 
	}

	bool isInDetectArea(VECTOR v)
	{
		if (detectRange)
			return detectRange->isIn(v);
		return false;
	}

	~Enemy()
	{
		collider.unset();

		if (detectRange)
			delete detectRange;
		if (atkRange)
			delete atkRange;
		if (chaseRange)
			delete chaseRange;
	}

public:
	Enemy(POINT p, POINT v, const string spriteName, DIRECTION dir, int size, int hp = 40);
};

class EnemyIDLE : public StateMachine
{
private:
	TIMER timer;
	TIMER patrolTimer;

public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class EnemyOnMove : public StateMachine
{
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class EnemyPatrol : public EnemyOnMove
{
	TIMER patrolTimer;
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
};

class EnemyChase : public EnemyOnMove
{
private:
	TIMER pathUpdateTimer;

public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class EnemyKnockBack : public StateMachine
{
private:
	TIMER timer;

public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);

};

class EnemyAtk : public StateMachine
{
private:
	TIMER timer;
	bool hitten;

public:
	EnemyAtk() : timer(), hitten(false) {}
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};

class EnemyDead : public StateMachine
{
public:
	virtual StateMachine* setState(GameObject* obj, StateMachine* state);
	virtual void update(GameObject* obj, float dt);
	virtual void enter(GameObject* obj);
};