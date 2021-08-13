#pragma once

#ifndef __SPRITE__
#define __SPRITE__

#define TF_XFLIP (1)
#define TF_YFLIP (1 << 1)
#define TF_XYFLIP (1 << 2)

#include "framework.h"
#include <string>
#include <map>

using namespace Gdiplus;
using std::map;
using std::string;

class SpriteManager;
class MapManager;
class Autotile;

class Sprite
{
	friend class SpriteManager;

private:
	Image* img;
	int cx;
	int cy;
	int width;
	int height;

	Sprite() : img(NULL), cx(0), cy(0), width(0), height(0) {};

public:
	Sprite(const Sprite& ref) 
	{
		img = ref.img;
		cx = ref.cx; cy = ref.cy;
		width = ref.width; height = ref.height;
	}
	Sprite& operator=(const Sprite& ref)
	{
		img = ref.img;
		cx = ref.cx; cy = ref.cy;
		width = ref.width; height = ref.height;
	}

	void draw(const HDC&, int x, int y, int flag = 0);

	int getWidth() { return width; }
	int getHeight() { return height; }

};

class SpriteManager
{
	friend class MapManager;
	friend class Autotile;

private:
	SpriteManager();
	SpriteManager(const SpriteManager& ref) {}
	SpriteManager& operator=(const SpriteManager& ref) {}
	~SpriteManager() {};

	map<string, map<string, Rect>> dataMap;
	map<string, Image*> imageMap;
	map<string, Image*> tilesetMap;
	map<string, Image*> autotileMap;

	void loadImage(std::wstring path, std::string name);
	Gdiplus::Image* getTilesetSprite(std::string name) { return tilesetMap[name]; };
	Gdiplus::Image* getAutotileSprite(std::string name) { return autotileMap[name]; }


public:


	static SpriteManager& getInstance()
	{
		static SpriteManager s;
		return s;
	}

	void draw(const HDC&, string atlas, int x, int y, string sprite = "");

	Sprite getSprite(string imageFile, string subSprite = "");

	bool hasThisImage(string imageName);
};


#endif