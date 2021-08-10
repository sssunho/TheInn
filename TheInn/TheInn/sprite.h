#pragma once

#ifndef __SPRITE__
#define __SPRITE__

#include "framework.h"
#include <string>
#include <map>

using namespace Gdiplus;
using std::map;
using std::string;

class SpriteManager;
class MapManager;
class Autotile;

enum TransformType { NONE, FLIP_X, FLIP_Y };

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

	void draw(const HDC&, int x, int y, TransformType flag = TransformType::NONE);

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