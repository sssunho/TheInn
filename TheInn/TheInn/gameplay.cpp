#include "framework.h"
#include "gameplay.h"
#include "sprite.h"
#include "animator.h"
#include "map.h"
#include "gameObject.h"
#include "wndControl.h"
#include "coordTransform.h"
#include <ctime>

extern HWND ghWnd;
extern HDC hMainDC;
extern HDC hBufferDC;
extern HDC hBackBufferDC;
extern RECT clientRect;

void(*playerEvent)() = playerMove;

class Test
{
public:
	void foo() { static int i = 0; i++; }
};

void test()
{
	static Map* mmm = NULL;
	SpriteManager& s = SpriteManager::getInstance();
	AnimationManager& am = AnimationManager::getInstance();

	static clock_t time = clock();
	clock_t dt = clock() - time;

	MapManager& m = MapManager::getInstance();

	static Actor* actor = ObjectManager::createActor("actor", { 300,300 }, { 0, 0 }, "swordman");
	static Actor* enemy = ObjectManager::createActor("enemy", { 300, 350 }, { 0,0 }, "knight");
	actor->spriteOffset = { 0, -16 };
	enemy->spriteOffset = { 0, -16 };
	static bool test = true;

	if (test)
	{
		mmm = m.loadMap("debugZone.mapdata");
		actor->setState(ActorState::ONMOVE);
		test = false;
	}
	Camera& camera = Camera::getInstance();


	if (dt > 33)	// 30fps
	{
		time = clock();
		ObjectManager::update(dt / 1000.0f);
		Camera::Bound(actor->pos);
		mmm->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 0);

		mmm->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 1);
		

		ObjectManager::draw(hBufferDC);

		playerEvent();

		mmm->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 2);

		BitBlt(hMainDC, 0, 0, clientRect.right, clientRect.bottom, hBufferDC, 0, 0, SRCCOPY);
		
	}
}

void playerMove()
{
	static Actor* actor = ObjectManager::findActor("actor");

	DIRECTION key = getDirectionKeyState();
	ActorState state = actor->getState();

	if (key == DIRECTION::NONE && state == ActorState::ONMOVE)
		actor->setState(ActorState::IDLE);
	if (key != DIRECTION::NONE && state == ActorState::IDLE)
		actor->setState(ActorState::ONMOVE);

	if (key != actor->getDirection() && key != DIRECTION::NONE)
		actor->setDirection(key);

	float speed = 280.0f;
	float dirspeed = 200.0f;

	switch (key)
	{
	case DIRECTION::U:
		actor->vel.x = 0;
		actor->vel.y = -speed;
		break;
	case DIRECTION::LU:
		actor->vel.y = -dirspeed;
		actor->vel.x = -dirspeed;
		break;
	case DIRECTION::RU:
		actor->vel.y = -dirspeed;
		actor->vel.x = dirspeed;
		break;
	case DIRECTION::D:
		actor->vel.x = 0;
		actor->vel.y = speed;
		break;
	case DIRECTION::LD:
		actor->vel.y = dirspeed;
		actor->vel.x = -dirspeed;
		break;
	case DIRECTION::RD:
		actor->vel.y = dirspeed;
		actor->vel.x = dirspeed;
		break;
	case DIRECTION::L:
		actor->vel.y = 0;
		actor->vel.x = -speed;
		break;
	case DIRECTION::R:
		actor->vel.y = 0;
		actor->vel.x = speed;
		break;
	default:
		actor->vel.x = 0;
		actor->vel.y = 0;
		break;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
	{
		playerEvent = playerHit;
		actor->vel = { 0, 0 };
		actor->setState(ActorState::VSLASH);
		actor->unsetRepeat();
	}

}

void playerHit()
{
	static int combo = 0;
	static Actor* actor = ObjectManager::findActor("actor");
	static Animation ani = AnimationManager::getInstance().getAnimation("slash", "slashfxF", false);
	ani.update();
	POINT drawPoint = actor->pos;
	drawPoint = pixelToScreen(drawPoint);
	ani.draw(hBufferDC, drawPoint.x, drawPoint.y - 5);
	if (!ani.isPlaying())
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8001 && combo < 2)
		{
			if (combo == 0)
				actor->setState(ActorState::HSLASH);
			else if (combo == 1)
				actor->setState(ActorState::STAB);
			combo++;
			
		}
		else
		{
			ani.rewind();
			playerEvent = playerMove;
			actor->setState(ActorState::IDLE);
			combo = 0;
		}
	}
}

void run()
{

}