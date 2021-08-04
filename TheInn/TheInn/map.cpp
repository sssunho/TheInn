#include "map.h"
#include "framework.h"
#include "sprite.h"
#include "wndControl.h"
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;
extern HWND ghWnd;

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

Map* MapManager::loadMap()
{
	Bitmap* layer[3];

	layer[0] = new Bitmap(mapTable["test.mapdata"].nx * TILE_PIXEL, 
		mapTable["test.mapdata"].ny* TILE_PIXEL, PixelFormat32bppARGB);
	layer[1] = new Bitmap(mapTable["test.mapdata"].nx * TILE_PIXEL,
		mapTable["test.mapdata"].ny* TILE_PIXEL, PixelFormat32bppARGB);
	layer[2] = new Bitmap(mapTable["test.mapdata"].nx * TILE_PIXEL,
		mapTable["test.mapdata"].ny* TILE_PIXEL, PixelFormat32bppARGB);

	TilesetData tilesetData = tilesetTable[mapTable["test.mapdata"].tilesetName];
	SpriteManager& sm = SpriteManager::getInstance();
	Image* tilesetImg = sm.getTilesetSprite(tilesetTable[mapTable["test.mapdata"].tilesetName].spriteName);
	
	Rect drawDest = { 0, 0, TILE_PIXEL, TILE_PIXEL };

	Graphics canvas0(layer[0]);
	Graphics canvas1(layer[1]);
	Graphics canvas2(layer[2]);

	for(int i = 0 ; i < mapTable["test.mapdata"].ny ; i++)
		for (int j = 0; j < mapTable["test.mapdata"].nx; j++)
		{
			drawDest.X = j * TILE_PIXEL;
			drawDest.Y = i * TILE_PIXEL;
			canvas0.DrawImage(tilesetImg,
				drawDest,
				mapTable["test.mapdata"].cellData[0][i][j].x*TILE_PIXEL, 
				(mapTable["test.mapdata"].cellData[0][i][j].y - 1)*TILE_PIXEL,
				TILE_PIXEL, TILE_PIXEL,
				UnitPixel
			);
			canvas1.DrawImage(tilesetImg,
				drawDest,
				mapTable["test.mapdata"].cellData[1][i][j].x*TILE_PIXEL,
				(mapTable["test.mapdata"].cellData[1][i][j].y - 1)*TILE_PIXEL,
				TILE_PIXEL, TILE_PIXEL,
				UnitPixel
			);
			canvas2.DrawImage(tilesetImg,
				drawDest,
				mapTable["test.mapdata"].cellData[2][i][j].x*TILE_PIXEL,
				(mapTable["test.mapdata"].cellData[2][i][j].y - 1)*TILE_PIXEL,
				TILE_PIXEL, TILE_PIXEL,
				UnitPixel
			);
		}

	return new Map(layer[0], layer[1], layer[2]);
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