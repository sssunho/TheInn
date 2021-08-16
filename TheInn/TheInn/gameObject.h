#pragma once
#ifndef __GAMEOBJ__
#define __GAMEOBJ__

#include "VECTOR.h"
#include "geometry.h"
#include "framework.h"
#include "pointVector.h";
#include "animator.h"
#include "map.h"
#include <string>
#include <queue>

class StateMachine;

enum class DIRECTION { NONE = 0, D = 1, R = 2, RD = 3, U = 4, RU = 6, L = 8, LD = 9, LU = 12 };
enum class ActorState { IDLE, ONMOVE, VSLASH, HSLASH, STAB };

class GameObject
{
	friend class ObjectManager;
private:
	bool destroy;

protected:
	StateMachine* mState;

public:
	VECTOR pos;
	VECTOR vel;
	GameObject(POINT p = { 0, 0 }, POINT v = {0, 0}) : destroy(false), mState(nullptr)
	{
		pos.x = p.x;
		pos.y = p.y;
		vel.x = v.x;
		vel.y = v.y;
	}

	virtual ~GameObject()
	{
		if (mState)
			delete mState;
	}

	virtual void draw(HDC& hdc) = 0;
	virtual void update(float dt) { pos = dt * vel; };
	void destroyObj() { destroy = true; }

	void setState(StateMachine* state);
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

	int getSize() const { return size; }

	void set();
	void unset();
};

class Actor : public GameObject
{
	friend class StateMachine;
	friend void slashAtk(GameObject* obj, int dmg);

private:
	ActorState state;

protected:
	Collider collider;
	DIRECTION dir;
	string spriteName;
	Animation animation;

public:
	int MaxHP;
	int HP;
	Actor(POINT p = { 0, 0 }, 
		  POINT v = { 0, 0 }, 
		  const string spriteName = NULL, 
		  DIRECTION dir = DIRECTION::D,
		  int size = 1,
		  int hp = 40);

	virtual ~Actor() 
	{
		collider.unset();
	}

	POINT spriteOffset;

	virtual void setDirection(DIRECTION dir);
	DIRECTION getDirection() { return dir; }

	ActorState getState() { return state; }

	void setSprite(string name) { spriteName = name; }
	string getSpriteName() { return spriteName; }

	void setAnimation(string name, bool repeat = true);
	bool animationPlaying() { return animation.playing; }
	void setRepeat() { animation.repeat = true; }
	void unsetRepeat() { animation.repeat = false; }

	void setSpeed(float f);

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
	int flag;

public:
	SpriteFX(POINT p, const string spriteName, const string aniName, int flag = 0);

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
	static std::list<GameObject*> objList;

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

	static void createFX(std::string spriteName, std::string aniName, POINT pos, int flag = 0);

	static Actor* findActor(std::string name);

	static void registerObj(string, GameObject*);

	static void registerObj(GameObject*);

	static void draw(HDC& hdc);

	static void update(float dt);

	static void forEachInArea(Area* area, int param, void(*func)(GameObject*, int));

	static void releaseAll();

};

DIRECTION getDirectionFromVector(VECTOR v);

void drawHP(HDC & hdc, POINT p, int max, int hp);

#endif
