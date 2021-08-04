#pragma once
#ifndef __TILESET__
#define __TILESET__

#include "gridMap.h"
#include "framework.h"
#include <string>
#include <vector>

const int mapDataMask[4][4] = 
{
{1<<15, 1 << 14, 1 << 13, 1 << 12},
{1 << 11, 1 << 10, 1 << 9, 1 << 8},
{1 << 7, 1 << 6, 1 << 5, 1 << 4},
{1 << 3, 1 << 2, 1 << 1, 1}
};

class MapEditor;

class TilesetEditor
{
	friend class MapEditor;
private:
	HWND hWndOwner;
	std::wstring spriteName;
	Gdiplus::Image* sprite;
	Gdiplus::Bitmap* grid;
	POINT currentPos;
	POINT selectPos;
	std::vector<std::vector<int>>  tileData;
	TilesetEditor() {};
	Gdiplus::Image* getSprite() { return sprite; };

public:
	TilesetEditor(HWND owner, std::wstring path);

	~TilesetEditor()
	{
		delete sprite;
		delete grid;
	}
	void setCurrentPos(POINT mousePOS);
	POINT getCurrentPos() { return currentPos; }

	void setSelectPos(POINT mousePOS);
	POINT getSelectPos() { return selectPos; };

	void save();


	void render();
	void drawTile(HWND hWnd);
	void drawTileInfo(HWND hWnd, POINT p);
	void setTileData(POINT targetTile, POINT targetData);
};


#endif
