#include "framework.h"
#include "headers.h"
#include <ctime>

extern HWND ghWnd;
extern HDC hMainDC;
extern HDC hBufferDC;
extern HDC hBackBufferDC;
extern RECT clientRect;

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
	static PlayerBT bt(actor);

	if (test)
	{
		SoundManager::init();
		mmm = m.loadMap("debugZone.mapdata");
		actor->setState(ActorState::ONMOVE);
		test = false;
	}
	Camera& camera = Camera::getInstance();
	Actor* find = ObjectManager::findActor("enemy");

	static bool input = true;

	if (dt > 33)	// 30fps
	{
		time = clock();

		ObjectManager::update(dt / 1000.0f);
		Camera::Bound(actor->pos);
		mmm->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 0);

		mmm->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 1);
		
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8001 && input)
		{
			ObjectManager::createFX("slash", "slashfxF", actor->pos, DIRECTION::D);
			input = false;
		}
		if (!GetAsyncKeyState(VK_LBUTTON))
			input = true;

		ObjectManager::draw(hBufferDC);
		bt.Invoke();

		mmm->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 2);

		BitBlt(hMainDC, 0, 0, clientRect.right, clientRect.bottom, hBufferDC, 0, 0, SRCCOPY);
		
	}
}

void playerHit()
{/*
	static int combo = 0;
	static Actor* actor = ObjectManager::findActor("actor");
	static Animation ani = AnimationManager::getInstance().getAnimation("slash", "slashfxF", false);
	ani.update();
	POINT drawPoint = actor->pos;
	drawPoint = pixelToScreen(drawPoint);
	ani.draw(hBufferDC, drawPoint.x, drawPoint.y - 5);
	if (!ani.isPlaying())
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0&8001 && combo < 2)
		{
			if (combo == 0)
				actor->setState(ActorState::HSLASH);
			else if (combo == 1)
				actor->setState(ActorState::STAB);
			ani.rewind();
			combo++;
			
		}
		else
		{
			ani.rewind();
			playerEvent = playerMove;
			actor->setState(ActorState::IDLE);
			combo = 0;
		}
	}*/
}

void run()
{

}