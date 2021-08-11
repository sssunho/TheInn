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
enum class ActorState { IDLE, ONMOVE, VSLASH, HSLASH, STAB };

class GameObject
{
public:
	VECTOR pos;
	VECTOR vel;
	GameObject(POINT p = { 0, 0 }, POINT v = {0, 0})
	{
		pos.x = p.x;
		pos.y = p.y;
		vel.x = v.x;
		vel.y = v.y;
	}
	virtual void draw(HDC& hdc) = 0;
	virtual void update(float dt) { pos = dt * vel; };
};

class Collider : public GameObject
{
	friend class Actor;
private:
	GameObject* owner;
	int size;

public:
	Collider() {}
	Collider(GameObject* owner, int size) : owner(owner), size(size) {}
	bool collision();
	virtual void draw(HDC& hdc) {};
	virtual void update(float dt);

	void set();
	void unset();
};

class Actor : public GameObject
{
private:
	DIRECTION dir;
	ActorState state;
	string spriteName;
	Animation animation;
	Collider collider;

	int HP = 0;

public:
	Actor(POINT p = { 0, 0 }, 
		  POINT v = { 0, 0 }, 
		  const string spriteName = NULL, 
		  DIRECTION dir = DIRECTION::D,
		  int size = 1);

	POINT spriteOffset;

	void setDirection(DIRECTION dir);
	DIRECTION getDirection() { return dir; }

	void setState(ActorState st);
	ActorState getState() { return state; }

	bool animationPlaying() { return animation.playing; }
	void setRepeat() { animation.repeat = true; }
	void unsetRepeat() { animation.repeat = false; }

	virtual void draw(HDC& hdc);
	virtual void update(float dt);

};

class Camera : public GameObject
{
private:
	Camera() {};
	Camera(const Camera& ref) {}
	Camera& operator=(const Camera& ref) {}
	
	static int margin;
	GameObject* owner;

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

class ObjectManager
{
private:
	ObjectManager() {};
	ObjectManager(const ObjectManager& ref) {}
	MapManager& operator=(const MapManager& ref) {}

	static std::map<std::string, GameObject*> objTable;

public:
	static ObjectManager& getInstance()
	{
		static ObjectManager s;
		return s;
	}

	static Actor* createActor(std::string name, POINT pos, VECTOR vel, 
								   std::string SpriteName, DIRECTION dir = DIRECTION::D,
								   int size = 1);

	static Actor* findActor(std::string name);
	
	static void draw(HDC& hdc);

	static void update(float dt);
};

#endif