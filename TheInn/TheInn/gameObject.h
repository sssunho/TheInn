#pragma once
#ifndef __GAMEOBJ__
#define __GAMEOBJ__

#include "VECTOR.h"
#include "framework.h"
#include "pointVector.h";
#include "animator.h"
#include "map.h"
#include <string>
#include <queue>

enum class DIRECTION { NONE = 0, D = 1, R = 2, RD = 3, U = 4, RU = 6, L = 8, LD = 9, LU = 12 };
enum class ActorState { IDLE, ONMOVE, VSLASH, HSLASH, STAB };

class GameObject
{
	friend class ObjectManager;
private:
	bool destroy;
public:
	VECTOR pos;
	VECTOR vel;
	GameObject(POINT p = { 0, 0 }, POINT v = {0, 0}) : destroy(false)
	{
		pos.x = p.x;
		pos.y = p.y;
		vel.x = v.x;
		vel.y = v.y;
	}
	virtual void draw(HDC& hdc) = 0;
	virtual void update(float dt) { pos = dt * vel; };
	void destroyObj() { destroy = true; }
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
	friend class StateMachine;

private:
	DIRECTION dir;
	ActorState state;
	Collider collider;
	int HP = 0;

protected:
	string spriteName;
	Animation animation;

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

class SpriteFX : public GameObject
{
private:
	SpriteFX();
	string SpriteName;
	Animation animation;

public:
	SpriteFX(POINT p, const string spriteName, const string aniName, DIRECTION dir);

	virtual void update(float dt);
	virtual void draw(HDC& hdc);
};

class ObjectManager
{
private:
	ObjectManager() {};
	ObjectManager(const ObjectManager& ref) {}
	MapManager& operator=(const MapManager& ref) {}

	struct cmp
	{
		bool operator()(GameObject* obj1, GameObject* obj2)
		{
			return obj1->pos.y > obj2->pos.y;
		}
	};

	static std::map<std::string, GameObject*> objTable;
	static std::list<GameObject*> fxList;

	static std::priority_queue<GameObject*, vector<GameObject*>, cmp> pq;

public:
	static ObjectManager& getInstance()
	{
		static ObjectManager s;
		return s;
	}

	static Actor* createActor(std::string name, POINT pos, VECTOR vel, 
							  std::string SpriteName, DIRECTION dir = DIRECTION::D,
							  int size = 1);

	static void createFX(std::string spriteName, std::string aniName, POINT pos, DIRECTION dir);

	static Actor* findActor(std::string name);
	
	static void draw(HDC& hdc);

	static void update(float dt);
};

#endif