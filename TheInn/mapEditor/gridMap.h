#pragma once
#ifndef __GRIDMAP__
#define __GRIDMAP__

#define CELL_PIXEL_SIZE 8
#define TILE_DIM_CELL 4
#define TILE_DIM_PIXEL (CELL_PIXEL_SIZE * TILE_DIM_CELL)

#include "framework.h"
#include <vector>

using std::vector;
using namespace Gdiplus;

const int CELL_ID[TILE_DIM_CELL][TILE_DIM_CELL] = 
{
	{1 << 15, 1 << 14, 1 << 13, 1 << 12},
	{1 << 11, 1 << 10, 1 << 9, 1 << 8},
	{1 << 7, 1 << 6, 1 << 5, 1 << 4},
	{1 << 3, 1 << 2, 1 << 1, 1},
};

void loadTileSet(HWND hwnd, HWND hwndClient);

POINT toTileCoord(POINT p);

class TileSetEditor
{
private:
	POINT selectedTile;
	POINT mousePos;
	HWND hTileWnd;
	Image* tileSetImage;
	bool animation;
	struct TileData
	{
		int cellData;
		int layer;
	};

	vector<vector<TileData>> tileSetData;
	TileSetEditor() {}

public:
	TileSetEditor(HWND hWnd, Image* image, bool animation = false);

	void renderTileSet(HDC& hdc);
	void renderSelection(HDC& hdc, POINT pos);
	void selectTile(POINT pos);
	void setMousePos(POINT pos);

	~TileSetEditor() { delete tileSetImage; }

};

#endif // !__GRIDMAP__
