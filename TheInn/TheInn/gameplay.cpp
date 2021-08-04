#include "framework.h"
#include "gameplay.h"
#include "sprite.h"
#include "animator.h"
#include "map.h"
#include "gameObject.h"
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

	static Animation* ani = am.getAnimation("bowmasterWalkF");

	clock_t dt = clock() - time;

	MapManager& m = MapManager::getInstance();

	static bool test = true;
	if (test)
	{
		mmm = m.loadMap();
		test = false;
	}
	Camera& camera = Camera::getInstance();
	
	if (GetAsyncKeyState(VK_RIGHT) & 0x8001)
		camera.pos.x += 1;
	if (GetAsyncKeyState(VK_LEFT) & 0x8001)
		camera.pos.x += -1;
	if (GetAsyncKeyState(VK_UP) & 0x8001)
		camera.pos.y += -1;
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
		camera.pos.y += 1;

	if (dt > 17)	// 60fps
	{
		am.update();
		mmm->draw(hBufferDC, camera.pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 0);
		ani->draw(hBufferDC, 300, 300);
		BitBlt(hMainDC, 0, 0, clientRect.right, clientRect.bottom, hBufferDC, 0, 0, SRCCOPY);
	}
}

void run()
{

}