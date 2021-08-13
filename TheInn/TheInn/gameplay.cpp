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

	static Player* actor = new Player({ 300, 300 });
	static Actor* enemy = new Enemy({ 350, 350 }, { 0, 0 }, "slime", DIRECTION::D, 2);
	
	actor->spriteOffset = { 0, -16 };
	enemy->spriteOffset = { 0, -16 };
	static bool test = true;

	if (test)
	{
		SoundManager::init();
		mmm = m.loadMap("debugZone.mapdata");
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

		if (!GetAsyncKeyState(VK_LBUTTON))
			input = true;

		ObjectManager::draw(hBufferDC);

		mmm->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 2);

		BitBlt(hMainDC, 0, 0, clientRect.right, clientRect.bottom, hBufferDC, 0, 0, SRCCOPY);

	}
}

void run()
{

}

void find()
{
}