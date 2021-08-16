#include "framework.h"
#include "headers.h"
#include <ctime>

extern HWND ghWnd;
extern HDC hMainDC;
extern HDC hBufferDC;
extern HDC hBackBufferDC;
extern RECT clientRect;

extern void(*UPDATE)();

void(*EVENT)() = nullptr;

void run()
{
	Camera& camera = Camera::getInstance();
	static clock_t time = clock();
	clock_t dt = clock() - time;

	if (dt > 33)	// 30fps
	{
		time = clock();


		SoundManager::update();

		ObjectManager::update(dt / 1000.0f);
		

		if (EVENT)
			EVENT();

		if(GameObject* player = ObjectManager::findActor("player"))
			Camera::Bound(player->pos);
		
		MapManager::loadedMap->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 0);

		MapManager::loadedMap->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 1);

		ObjectManager::draw(hBufferDC);

		MapManager::loadedMap->draw(hBufferDC, Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }), 2);

		BitBlt(hMainDC, 0, 0, clientRect.right, clientRect.bottom, hBufferDC, 0, 0, SRCCOPY);

	}

}

void initStage1()
{
	MapManager& m = MapManager::getInstance();
	SpriteManager& s = SpriteManager::getInstance();
	AnimationManager& am = AnimationManager::getInstance();
	MapManager::loadMap("debugZone.mapdata");
	SoundManager::loadBGM("021-Field04.mid");
	SoundManager::playBGM();
	new Magician({ 300, 500 });
	new Magician({ 500, 500 });
	new Magician({ 700, 200 });
	new Magician({ 600, 350 });
	new Magician({ 1000, 800 });
	new Magician({ 1000, 800 });
	new Magician({ 1000, 800 });
	new Magician({ 1000, 1700 });
	new Magician({ 1000, 1500 });
	new Magician({ 500, 1500 });
	new Magician({ 300, 1500 });
	new Enemy({ 1000, 800 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1100, 800 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1500, 300 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 300, 550 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 700, 800 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1300, 200 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1300, 100 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1000, 800 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1400, 500 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 300, 1300 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1700, 1700 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1750, 1700 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1500, 1700 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 1400, 1700 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 500, 1500 }, { 0,0 }, "slime", DIRECTION::D, 2);
	new Enemy({ 200, 1600 }, { 0,0 }, "slime", DIRECTION::D, 2);
	if(!ObjectManager::findActor("Player"))
		new Player({ 1700, 300 });
	UPDATE = run;
	EVENT = enterCave;
}

void enterCave()
{
	GameObject* player = ObjectManager::findActor("player");
	if (player->pos.x > 1570 && player->pos.x <= 1630 && player->pos.y < 153)
	{
		SoundManager::playSE("Move.ogg");
		SoundManager::releaseBGM();
		releaseStage();
		initStage2();
	}
}

void releaseStage()
{
	ObjectManager::releaseAll();
	delete MapManager::loadedMap;
}

void initStage2()
{
	MapManager& m = MapManager::getInstance();
	SpriteManager& s = SpriteManager::getInstance();
	AnimationManager& am = AnimationManager::getInstance();
	MapManager::loadMap("cave.mapdata");
	SoundManager::loadBGM("035-Dungeon01.mid");
	SoundManager::playBGM();
	GameObject* obj = ObjectManager::findActor("player");

	new Magician({ 400, 500 });
	new Magician({ 600, 500 });
	new Magician({ 400, 550 });
	new Magician({ 400, 600 });
	new Magician({ 600, 550 });
	new Magician({ 600, 600 });
	new Magician({ 500, 600 });


	if (obj == NULL)
		new Player({ 300, 300 });
	else
	{
		obj->pos = { 490, 540 };
		obj->vel = { 0,0 };
	}
	
	UPDATE = run;
	EVENT = enterCave;

}