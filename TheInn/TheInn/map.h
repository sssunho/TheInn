#pragma once
#ifndef __MAP__
#define __MAP__
#define TILE_PIXEL 32
#define TILE_CELL 4
#define CELL_PIXEL 8
#define NUMS_MAP_LAYER 3

#include "framework.h"
#include "sprite.h"
#include "autoTile.h"
#include <map>
#include <string>
#include <vector>

class Autotile;
class MapManager;
class Node;

class Map
{
	friend class MapManager;
private:
	int cellNx;
	int cellNy;
	string mapName;
	Bitmap* grid;
	Bitmap* layer[3];
	std::vector<std::vector<int>> blockMap;
	std::vector<std::vector<int>> objectMap;
	std::vector<std::vector<Node*>> graph;

public:
	Map(string name, Bitmap* l1, Bitmap* l2, Bitmap* l3, Bitmap* grid = NULL);

	struct MapData
	{
		std::string tilesetName;
		int nx; int ny;
		std::vector<std::vector<POINT>> cellData[3];
	};

	void draw(HDC& hdc, POINT pos, int layer);

	int getWidth() const { return layer[0]->GetWidth(); }
	int getHeight() const { return layer[0]->GetHeight(); }

	bool isBlock(POINT p) const;
	void setBlock(POINT p);
	void unsetBlock(POINT p);

	bool isBlock(POINT p, int size) const;
	void setBlock(POINT p, int size);
	void unsetBlock(POINT p, int size);

	friend Node* findPath(POINT start, POINT end, int size);
	friend void updateNode(POINT pos, POINT end, POINT prev, int size);

	~Map()
	{
		delete grid;
		delete layer[0];
		delete layer[1];
		delete layer[2];
	}
};

class MapManager
{
	friend class Map;

private:
	MapManager();
	MapManager(const MapManager& ref) {}
	MapManager& operator=(const MapManager& ref) {}


	struct TilesetData
	{
		std::string spriteName;
		Autotile autotile[7];
		int nx; int ny;
		std::vector<std::vector<int>> cellData;
	};

	static std::map<std::string, TilesetData> tilesetTable;
	static std::map<std::string, Map::MapData> mapTable;

	static void initMapTable();
	static void initTilesetTable();

	static void drawUnitTile(Graphics& g, string mapName, int layer, POINT& srcPoint, Rect& dest);
	
public:

	static MapManager& getInstance()
	{
		static MapManager s;
		return s;
	}
	
	static Map* loadedMap;
	static Map* loadMap(string mapName);
	static POINT getLayerData(string mapName, int layer, POINT p)
	{
		if (p.x >= mapTable[mapName].cellData[0][0].size() || p.y >= mapTable[mapName].cellData[0].size()
			|| p.x < 0 || p.y < 0)
			return { -1, -1 };
		return mapTable[mapName].cellData[layer][p.y][p.x];
	}
};

#endif