#pragma once;
#ifndef __MAPEDITWND__
#define __MAPEDITWND__

#include "framework.h"
#include "tileSetEditor.h"
#include <string>
#include <vector>

class MapEditor
{
private:
	HWND hMain;
	HWND hTileset;
	TilesetEditor* tileWnd;
	Gdiplus::Bitmap* layer[3];
	Gdiplus::Graphics* canvas[3];
	POINT viewPoint;

	std::wstring tilesetName;
	int nx;
	int ny;
	int selectedLayer;

	std::vector<POINT> layerData[3];

	MapEditor() {};


public:
	MapEditor(HWND main, HWND tileset, const std::wstring path);
	~MapEditor()
	{
		delete canvas[0];
		delete canvas[1];
		delete canvas[2];
		delete layer[0];
		delete layer[1];
		delete layer[2];
		delete tileWnd;
	}
	
	TilesetEditor* getTileWnd() { return tileWnd; }
	void setLayer(int i) { selectedLayer = i; }
	void render();
	void drawUnitTile(POINT mousePos);
	void drawTile(POINT mousePos);
	void drawArea(POINT mousePos);
	POINT getTileCoord(POINT mousePos);
	void setSize(int x, int y);

	POINT getViewPoint() { return viewPoint; }
	void setViewPoint(POINT p);
	
	void drawArea(POINT p1, POINT p2);

	void drawAutoTile(Gdiplus::Graphics& g, Autotile* tile, POINT dest);

	void updateNeighbor(POINT dest);

	void save();

	POINT getData(POINT p);

	int getWidth() { return nx * TILE_PIXEL; }
	int getHeight() { return ny * TILE_PIXEL; }
	int getNx() { return nx; }
	int getNy() { return ny; }

	HWND getMainhWnd() { return hMain; }
};

#endif // !__MAPEDITWND__
