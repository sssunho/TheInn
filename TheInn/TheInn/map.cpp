#include "map.h"
#include "framework.h"
#include "sprite.h"
#include "wndControl.h"
#include "pointVector.h"
#include "autoTile.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;
using namespace Gdiplus;
extern HWND ghWnd;

extern HDC hMainDC;
extern HDC hBufferDC;
extern HDC hBackBufferDC;

map<string, MapManager::TilesetData> MapManager::tilesetTable;
map<string, Map::MapData> MapManager::mapTable;
Map* MapManager::loadedMap = NULL;

int cellMask[4][4] =
{
	{1 << 15, 1 << 14, 1 << 13, 1 << 12},
	{1 << 11, 1 << 10, 1 << 9, 1 << 8},
	{1 << 7, 1 << 6, 1 << 5, 1 << 4},
	{1 << 3, 1 << 2, 1 << 1, 1}
};


MapManager::MapManager()
{
	SpriteManager& s = SpriteManager::getInstance();

	initMapTable();
	initTilesetTable();
	
	wfstream mapDataFile;
	
}

void MapManager::initMapTable()
{
	SpriteManager& s = SpriteManager::getInstance();

	int i, j;

	vector<wstring> mapList;
	GetFiles(mapList, L"Data\\Map", false);
	for (i = 0; i < mapList.size(); i++)
	{
		fstream mapDataFile;
		mapDataFile.open(mapList[i]);
		if (mapDataFile.is_open())
		{
			Map::MapData mapData;
			fstream tilesetFile;

			mapDataFile >> mapData.tilesetName
						>> mapData.nx >> mapData.ny;

			mapData.cellData[0].resize(mapData.ny);
			mapData.cellData[1].resize(mapData.ny);
			mapData.cellData[2].resize(mapData.ny);
			for (int i = 0; i < mapData.cellData[0].size(); i++)
			{
				mapData.cellData[0][i].resize(mapData.nx);
				mapData.cellData[1][i].resize(mapData.nx);
				mapData.cellData[2][i].resize(mapData.nx);
			}

			for(int i = 0 ; i < mapData.ny ; i++)
				for (int j = 0; j < mapData.nx; j++)
				{
					mapDataFile >> mapData.cellData[0][i][j].x
								>> mapData.cellData[0][i][j].y;
				}

			for (int i = 0; i < mapData.ny; i++)
				for (int j = 0; j < mapData.nx; j++)
				{
					mapDataFile >> mapData.cellData[1][i][j].x
						>> mapData.cellData[1][i][j].y;
				}
			for (int i = 0; i < mapData.ny; i++)
				for (int j = 0; j < mapData.nx; j++)
				{
					mapDataFile >> mapData.cellData[2][i][j].x
						>> mapData.cellData[2][i][j].y;
				}
			string multiByteName;
			string subStr;
			multiByteName.assign(mapList[i].begin(), mapList[i].end());
			for (j = multiByteName.size(); j >= 0 && multiByteName[j] != '\\'; j--);
			subStr = multiByteName.c_str() + j + 1;
			mapTable.insert(pair<string, Map::MapData>(subStr, mapData));
			
			mapDataFile.close();
		}
	}
}

void MapManager::initTilesetTable()
{
	SpriteManager& s = SpriteManager::getInstance();

	int i, j;

	vector<wstring> tilesetList;
	GetFiles(tilesetList, L"Data\\Tilesets", false);
	for (i = 0; i < tilesetList.size(); i++)
	{
		fstream tilesetDataFile;
		tilesetDataFile.open(tilesetList[i]);
		if (tilesetDataFile.is_open())
		{
			TilesetData tilesetData;
			fstream tilesetFile;

			tilesetDataFile >> tilesetData.spriteName
					>> tilesetData.nx >> tilesetData.ny;
			
			char autotileName[512];
			stringstream wss;
			int stringSize, autotileCount = 0;
			tilesetDataFile.get();
			tilesetDataFile.getline(autotileName, 512);
			wss << autotileName;
			stringSize = strlen(autotileName);
			while (wss.tellg() < stringSize - 1)
			{
				int i = 0;
				wss >> autotileName;
				tilesetData.autotile[autotileCount].ID = POINT({ autotileCount + 1, 0 });
				for (i = strlen(autotileName) - 1; i >= 0 && autotileName[i] != '/' && autotileName[i] != '\\'; i--);
				tilesetData.autotile[autotileCount++].name = autotileName + i + 1;
			}

			tilesetData.cellData.resize(tilesetData.ny);
			for (int i = 0; i < tilesetData.cellData.size(); i++)
				tilesetData.cellData[i].resize(tilesetData.nx);

			for (int i = 0; i < tilesetData.ny; i++)
				for (int j = 0; j < tilesetData.nx; j++)
				{
					tilesetDataFile >> tilesetData.cellData[i][j];
				}

			string multiByteName;
			string subStr;
			multiByteName.assign(tilesetList[i].begin(), tilesetList[i].end());
			for (j = multiByteName.size(); j >= 0 && multiByteName[j] != '\\'; j--);
			subStr = multiByteName.c_str() + j + 1;
			tilesetTable.insert(pair<string, TilesetData>(subStr, tilesetData));

			tilesetDataFile.close();
		}
	}
}

void MapManager::drawUnitTile(Graphics & g, string mapName, int layer, POINT & srcPoint, Rect & dest)
{
	if (srcPoint.y)
	{
		SpriteManager& sm = SpriteManager::getInstance();
		Image* srcImg = sm.getTilesetSprite(tilesetTable[mapTable[mapName].tilesetName].spriteName);
		g.DrawImage(
			srcImg,
			dest,
			srcPoint.x  * TILE_PIXEL,
			(srcPoint.y - 1) * TILE_PIXEL,
			TILE_PIXEL, TILE_PIXEL,
			UnitPixel );
	}
	else
	{
		if (srcPoint.x)
		{
			tilesetTable[mapTable[mapName].tilesetName].autotile[srcPoint.x - 1].draw(g, layer, dest, mapName);
		}
	}
}

Map* MapManager::loadMap(string mapName)
{
	Bitmap* layer[3];
	Bitmap* grid;
	string tilesetName = mapTable[mapName].tilesetName;

	layer[0] = new Bitmap(mapTable[mapName].nx * TILE_PIXEL, 
		mapTable[mapName].ny* TILE_PIXEL, PixelFormat32bppARGB);
	layer[1] = new Bitmap(mapTable[mapName].nx * TILE_PIXEL,
		mapTable[mapName].ny* TILE_PIXEL, PixelFormat32bppARGB);
	layer[2] = new Bitmap(mapTable[mapName].nx * TILE_PIXEL,
		mapTable[mapName].ny* TILE_PIXEL, PixelFormat32bppARGB);
	grid = new Bitmap(mapTable[mapName].nx * TILE_PIXEL,
		mapTable[mapName].ny* TILE_PIXEL, PixelFormat32bppARGB);
	
	const TilesetData& tilesetData = tilesetTable[mapTable[mapName].tilesetName];
	SpriteManager& sm = SpriteManager::getInstance();
	Image* tilesetImg = sm.getTilesetSprite(tilesetTable[tilesetName].spriteName);
	
	Rect drawDest = { 0, 0, TILE_PIXEL, TILE_PIXEL };

	Graphics canvas0(layer[0]);
	Graphics canvas1(layer[1]);
	Graphics canvas2(layer[2]);
	Graphics g(grid);
	Pen gridPen(Color(255, 0, 0), 3);
	Pen cellPen(Color(0, 0, 255), 1);

	Map* loadedMap = new Map(mapName, layer[0], layer[1], layer[2], grid);
	loadedMap->blockMap.resize(mapTable[mapName].ny);

	for (int i = 0; i < loadedMap->blockMap.size(); i++)
		loadedMap->blockMap[i].resize(mapTable[mapName].nx);

	for (int i = 0; i < mapTable[mapName].ny; i++)
	{
		for (int j = 0; j < mapTable[mapName].nx; j++)
		{
			drawDest.X = j * TILE_PIXEL;
			drawDest.Y = i * TILE_PIXEL;
			drawUnitTile(canvas0, mapName, 0, mapTable[mapName].cellData[0][i][j], drawDest);

			loadedMap->blockMap[i][j] = tilesetData.cellData[mapTable[mapName].cellData[0][i][j].y][mapTable[mapName].cellData[0][i][j].x];

			drawUnitTile(canvas1, mapName, 1, mapTable[mapName].cellData[1][i][j], drawDest);
			
			if (mapTable[mapName].cellData[1][i][j] != POINT({0, 0}))
				loadedMap->blockMap[i][j] = tilesetData.cellData[mapTable[mapName].cellData[1][i][j].y][mapTable[mapName].cellData[1][i][j].x];


			drawUnitTile(canvas2, mapName, 2, mapTable[mapName].cellData[2][i][j], drawDest);

			if (mapTable[mapName].cellData[2][i][j] != POINT({ 0, 0 }))
				loadedMap->blockMap[i][j] = tilesetData.cellData[mapTable[mapName].cellData[2][i][j].y][mapTable[mapName].cellData[2][i][j].x];
/*
			g.DrawLine(&gridPen, Point(j * TILE_PIXEL, 0), Point(j*TILE_PIXEL,
				mapTable[mapName].ny * TILE_PIXEL));
			g.DrawLine(&cellPen, Point(j * TILE_PIXEL + 8, 0), Point(j*TILE_PIXEL + 8,
				mapTable[mapName].ny * TILE_PIXEL));
			g.DrawLine(&cellPen, Point(j * TILE_PIXEL + 16, 0), Point(j*TILE_PIXEL + 16,
				mapTable[mapName].ny * TILE_PIXEL));
			g.DrawLine(&cellPen, Point(j * TILE_PIXEL + 24, 0), Point(j*TILE_PIXEL + 24,
				mapTable[mapName].ny * TILE_PIXEL));*/

		}/*
		g.DrawLine(&gridPen, Point(0, i * TILE_PIXEL), Point(mapTable[mapName].nx * TILE_PIXEL,
			i*TILE_PIXEL));
		g.DrawLine(&cellPen, Point(0, i * TILE_PIXEL + 8), Point(mapTable[mapName].nx * TILE_PIXEL,
			i*TILE_PIXEL + 8));
		g.DrawLine(&cellPen, Point(0, i * TILE_PIXEL + 16), Point(mapTable[mapName].nx * TILE_PIXEL,
			i*TILE_PIXEL + 16));
		g.DrawLine(&cellPen, Point(0, i * TILE_PIXEL + 24), Point(mapTable[mapName].nx * TILE_PIXEL,
			i*TILE_PIXEL + 24));*/
	}

	MapManager::loadedMap = loadedMap;
	return loadedMap;
}

Map::Map(string name, Bitmap * l1, Bitmap * l2, Bitmap * l3, Bitmap * grid) : mapName(name)
{
	layer[0] = l1; layer[1] = l2; layer[2] = l3; Map::grid = grid;
}

void Map::draw(HDC& hdc, POINT pos, int l)
{
	Graphics g(hdc);
	RECT clientRect;
	GetClientRect(ghWnd, &clientRect);
	if(l == 0)
		g.FillRectangle(&SolidBrush(Color(255, 255, 255)),
			Rect({ 0, 0, clientRect.right, clientRect.bottom }));
	g.DrawImage(layer[l],
		Rect({ 0, 0, clientRect.right, clientRect.bottom }),
		pos.x, pos.y, clientRect.right, clientRect.bottom, UnitPixel);
}

bool Map::isBlock(POINT p)
{
	if (p.x < 0 || p.y < 0 ||
		p.x >= layer[0]->GetWidth() || p.y >= layer[0]->GetHeight())
		return true;
	
	return blockMap[p.y / TILE_PIXEL][p.x / TILE_PIXEL] & cellMask[(p.y / CELL_PIXEL) % TILE_CELL][(p.x / CELL_PIXEL) % TILE_CELL];
}
