#pragma once
#ifndef __GAMEOBJ__
#define __GAMEOBJ__

#include "VECTOR.h"
#include "framework.h"
#include "pointVector.h";
#include "animator.h"
#include "map.h"
#include <string>

enum class DIRECTION { NONE = 0, D = 1, R = 2, RD = 3, U = 4, RU = 6, L = 8, LD = 9, LU = 12 };
enum class ActorState { IDLE, ONMOVE };

class GameObject
{
public:
	VECTOR realPos;
public:
	POINT pos;
	VECTOR vel;
	GameObject(int x = 0, int y = 0, int vx = 0, int vy = 0) 
		: pos(POINT({ x,y })), realPos(VECTOR({ (float)x, (float)y })), vel(VECTOR({(float)vx, (float)vy}))
	{
	}

	virtual void draw(HDC& hdc) = 0;
	virtual void update(float dt) { realPos = dt * vel; pos = realPos; };
};

class Actor : public GameObject
{
private:
	DIRECTION dir;
	ActorState state;
	string spriteName;
	Animation animation;

public:
	Actor(int x = 0, int y = 0, DIRECTION dir = DIRECTION::D, const string spriteName = NULL);

	POINT spriteOffset;

	void setDirection(DIRECTION dir);
	DIRECTION getDirection() { return dir; }

	void setState(ActorState st);
	ActorState getState() { return state; }

	virtual void draw(HDC& hdc);
	virtual void update(float dt);

};

class Camera : public GameObject
{
private:
	Camera() : margin(TILE_PIXEL) {};
	Camera(const Camera& ref) {}
	Camera& operator=(const Camera& ref) {}
	
	int margin;

public:

	static POINT pos;

	virtual void draw(HDC& hdc) {}
	virtual void update(float dt) { GameObject::update(dt); };

	static Camera& getInstance()
	{
		static Camera s;
		return s;
	}

	static bool isIn(POINT p);

	static POINT getPos() { return Camera::pos; }

	static void Bound(POINT p);

};

class Collider : public GameObject
{
private:
	GameObject* owner;
	int size;

public:
	bool collision();
};


#endif