#include "framework.h"
#include "gameplay.h"
#include "sprite.h"
#include "animator.h"

extern HWND ghWnd;

void test()
{
	SpriteManager& s = SpriteManager::getInstance();
	AnimationManager& ani = AnimationManager::getInstance();
	HDC hdc = GetDC(ghWnd);
	//s.draw(hdc, "bowmaster", 100, 100, "standF");

	Sprite sp = s.getSprite("bowmaster", "standF");

	sp.draw(hdc, 100, 100);

	Animation a = ani.getAnimation("");

	ReleaseDC(ghWnd, hdc);
}

void run()
{

}