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
class Autotile;

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
	Autotile* autoTile[7];
	std::vector<std::vector<int>>  tileData;
	TilesetEditor() {};
	Gdiplus::Image* getSprite() { return sprite; };

	
	POINT dragStart;
	POINT dragEnd;
	bool dragArea = false;

public:
	TilesetEditor(HWND owner, std::wstring path);

	~TilesetEditor();
	void setCurrentPos(POINT mousePOS);
	POINT getCurrentPos() { return currentPos; }

	void selectTile(POINT mousePOS);

	void setSelectPos(POINT pos) { selectPos = pos; }
	POINT getSelectPos() { return selectPos; };

	POINT getAreaStart() { return dragStart; }
	POINT getAreaEnd() { return dragEnd; }

	Autotile* getAutoTile() { return autoTile[selectPos.x - 1]; }

	void save();
	bool isAreaDraw() { return dragArea; }


	void render();
	void drawTile(HWND hWnd);
	void drawTileInfo(HWND hWnd, POINT p);
	void setTileData(POINT targetTile, POINT targetData);

	void selectTile(POINT p1, POINT p2);

	std::wstring getAutotilePath(int i);

	void loadAutoTile(int i, std::wstring path);

};


#endif
