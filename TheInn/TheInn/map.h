#pragma once
#ifndef __MAP__
#define __MAP__
#define TILE_PIXEL 32
#define NUMS_MAP_LAYER 3

#include "framework.h"
#include "sprite.h"
#include <map>
#include <string>
#include <vector>

class MapManager;

class Map
{
	friend class MapManager;
private:
	Bitmap* layer[3];
public:
	Map(Bitmap* l1, Bitmap* l2, Bitmap* l3) { layer[0] = l1; layer[1] = l2; layer[2] = l3; }

	struct MapData
	{
		std::string tilesetName;
		int nx; int ny;
		std::vector<std::vector<POINT>> cellData[3];
	};

	void draw(HDC& hdc, POINT pos, int layer);

	~Map()
	{
		delete layer[0];
		delete layer[1];
		delete layer[2];
	}
};

class MapManager
{
private:
	MapManager();
	MapManager(const MapManager& ref) {}
	MapManager& operator=(const MapManager& ref) {}


	struct TilesetData
	{
		std::string spriteName;
		int nx; int ny;
		std::vector<std::vector<int>> cellData;
	};

	std::map<std::string, TilesetData> tilesetTable;
	std::map<std::string, Map::MapData> mapTable;

	void initMapTable();
	void initTilesetTable();
	
public:

	static MapManager& getInstance()
	{
		static MapManager s;
		return s;
	}
	
	Map* loadMap();

};

#endif