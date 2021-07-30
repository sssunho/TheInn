#include "framework.h"
#include "gameplay.h"
#include "sprite.h"
#include "animator.h"
#include <ctime>

extern HWND ghWnd;

void test()
{
	SpriteManager& s = SpriteManager::getInstance();
	AnimationManager& am = AnimationManager::getInstance();
	HDC hdc = GetDC(ghWnd);
	//s.draw(hdc, "bowmaster", 100, 100, "standF");
/*
	Sprite sp = s.getSprite("bowmaster", "standF");

	sp.draw(hdc, 100, 100);*/

	static clock_t time = clock();

	static Animation* ani = am.getAnimation("bowmasterWalkF");

	clock_t dt = clock() - time;

	if (dt > 17)	// 60fps
	{
		am.update();
		ani->draw(hdc, 300, 300);

		time = clock();
	}
	
	ReleaseDC(ghWnd, hdc);
}

void run()
{

}