#include "framework.h"
#include "gameplay.h"
#include "sprite.h"
#include "animator.h"
#include "map.h"
#include "gameObject.h"
#include "wndControl.h"
#include <ctime>

extern HWND ghWnd;
extern HDC hMainDC;
extern HDC hBufferDC;
extern RECT clientRect;

void test()
{
	static Map* mmm = NULL;
	SpriteManager& s = SpriteManager::getInstance();
	AnimationManager& am = AnimationManager::getInstance();

	static clock_t time = clock();
	clock_t dt = clock() - time;

	MapManager& m = MapManager::getInstance();

	static Actor actor(0, 0, DIRECTION::R, "swordman");
	static bool test = true;
	if (test)
	{
		mmm = m.loadMap();
		actor.setState(ActorState::ONMOVE);
		test = false;
	}
	Camera& camera = Camera::getInstance();
	


	if (dt > 17)	// 60fps
	{
		DIRECTION key = getDirectionKeyState();
		ActorState state = actor.getState();
		
		if (key == DIRECTION::NONE && state == ActorState::ONMOVE)
			actor.setState(ActorState::IDLE);
		if (key != DIRECTION::NONE && state == ActorState::IDLE)
			actor.setState(ActorState::ONMOVE);

		if (key != actor.getDirection() && key != DIRECTION::NONE)
			actor.setDirection(key);

		int speed = 7;
		int dirspeed = 5;

		switch (key)
		{
		case DIRECTION::U:
			actor.pos.y -= speed;
			break;
		case DIRECTION::LU:
			actor.pos.y -= dirspeed;
			actor.pos.x -= dirspeed;
			break;
		case DIRECTION::RU:
			actor.pos.y -= dirspeed;
			actor.pos.x += dirspeed;
			break;
		case DIRECTION::D:
			actor.pos.y += speed;
			break;
		case DIRECTION::LD:
			actor.pos.y += dirspeed;
			actor.pos.x -= dirspeed;
			break;
		case DIRECTION::RD:
			actor.pos.y += dirspeed;
			actor.pos.x += dirspeed;
			break;
		case DIRECTION::L:
			actor.pos.x -= speed;
			break;
		case DIRECTION::R:
			actor.pos.x += speed;
			break;
		}

		mmm->draw(hBufferDC, camera.pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 0);

		actor.update(dt);
		camera.pos = actor.pos;


		actor.draw(hBufferDC);

		BitBlt(hMainDC, 0, 0, clientRect.right, clientRect.bottom, hBufferDC, 0, 0, SRCCOPY);
		time = clock();
	}
}

void run()
{

}