#include "autoTile.h"
#include "pointVector.h"
#include "coordTransform.h"

using namespace Gdiplus;

void Autotile::drawLU(POINT l, POINT r, POINT u, POINT d, POINT lu)
{
	if (ID == u && ID == l)
	{
		if (ID == lu)
		{
			cellData[0].x = 2;
			cellData[0].y = 4;
		}
		else
		{
			cellData[0].x = 4;
			cellData[0].y = 0;
		}
	}
	else if (ID == u && ID != l)
	{
		cellData[0].x = 0;
		if (ID == d)
		{
			cellData[0].y = 4;
		}
		else
		{
			cellData[0].y = 6;
		}
	}
	else if (ID != u && ID == l)
	{
		cellData[0].y = 2;
		if (ID == r)
		{
			cellData[0].x = 2;
		}
		else
		{
			cellData[0].x = 4;
		}
	}
	else
	{
		cellData[0].x = 0;
		cellData[0].y = 2;
	}
}

void Autotile::drawRU(POINT l, POINT r, POINT u, POINT d, POINT ru)
{

	if (ID == u && ID == r)
	{
		if (ID == ru)
		{
			cellData[1].x = 3;
			cellData[1].y = 4;
		}
		else
		{
			cellData[1].x = 5;
			cellData[1].y = 0;
		}
	}
	else if (ID == u && ID != r)
	{
		cellData[1].x = 5;
		if (ID == d)
		{
			cellData[1].y = 4;
		}
		else
		{
			cellData[1].y = 6;
		}
	}
	else if (ID != u && ID == r)
	{
		cellData[1].y = 2;
		if (ID == l)
		{
			cellData[1].x = 3;
		}
		else
		{
			cellData[1].x = 1;
		}
	}
	else
	{
		cellData[1].x = 5;
		cellData[1].y = 2;
	}
}

void Autotile::drawLD(POINT l, POINT r, POINT u, POINT d, POINT ld)
{
	if (ID == d && ID == l)
	{
		if (ID == ld)
		{
			cellData[2].x = 2;
			cellData[2].y = 5;
		}
		else
		{
			cellData[2].x = 4;
			cellData[2].y = 1;
		}
	}
	else if (ID == d && ID != l)
	{
		cellData[2].x = 0;
		if (ID == u)
		{
			cellData[2].y = 5;
		}
		else
		{
			cellData[2].y = 3;
		}
	}
	else if (ID != d && ID == l)
	{
		cellData[2].y = 7;
		if (ID == r)
		{
			cellData[2].x = 2;
		}
		else
		{
			cellData[2].x = 4;
		}
	}
	else
	{
		cellData[2].x = 0;
		cellData[2].y = 7;
	}
}

void Autotile::drawRD(POINT l, POINT r, POINT u, POINT d, POINT rd)
{
	if (ID == d && ID == r)
	{
		if (ID == rd)
		{
			cellData[3].x = 3;
			cellData[3].y = 5;
		}
		else
		{
			cellData[3].x = 5;
			cellData[3].y = 1;
		}
	}
	else if (ID == d && ID != r)
	{
		cellData[3].x = 5;
		if (ID == u)
		{
			cellData[3].y = 5;
		}
		else
		{
			cellData[3].y = 3;
		}
	}
	else if (ID != d && ID == r)
	{
		cellData[3].y = 7;
		if (ID == l)
		{
			cellData[3].x = 3;
		}
		else
		{
			cellData[3].x = 1;
		}
	}
	else
	{
		cellData[3].x = 5;
		cellData[3].y = 7;
	}
}

void Autotile::draw(Graphics & g, int layer, Rect & dest, std::string mapName)
{

	SpriteManager& sm = SpriteManager::getInstance();
	POINT drawPoint = { dest.X, dest.Y };
	Image* sprite = sm.getAutotileSprite(name);
	drawPoint = pixelToTile(drawPoint);

	POINT l = MapManager::getLayerData(mapName, layer, drawPoint + POINT({ -1, 0 }));
	POINT r = MapManager::getLayerData(mapName, layer, drawPoint + POINT({ 1, 0 }));
	POINT u = MapManager::getLayerData(mapName, layer, drawPoint + POINT({ 0, -1 }));
	POINT d = MapManager::getLayerData(mapName, layer, drawPoint + POINT({ 0, 1 }));
	POINT lu = MapManager::getLayerData(mapName, layer, drawPoint + POINT({ -1, -1 }));
	POINT ru = MapManager::getLayerData(mapName, layer, drawPoint + POINT({ 1, -1 }));
	POINT ld = MapManager::getLayerData(mapName, layer, drawPoint + POINT({ -1, 1 }));
	POINT rd = MapManager::getLayerData(mapName, layer, drawPoint + POINT({ 1, 1 }));

	Rect luRect = { dest.X, dest.Y, TILE_PIXEL / 2, TILE_PIXEL / 2 };
	Rect ruRect = { dest.X + TILE_PIXEL / 2, dest.Y, TILE_PIXEL / 2, TILE_PIXEL / 2 };
	Rect ldRect = { dest.X, dest.Y + TILE_PIXEL / 2,  TILE_PIXEL / 2, TILE_PIXEL / 2 };
	Rect rdRect = { dest.X + TILE_PIXEL / 2,dest.Y + TILE_PIXEL / 2, TILE_PIXEL / 2, TILE_PIXEL / 2 };

	drawLU(l, r, u, d, lu);
	drawRU(l, r, u, d, ru);
	drawLD(l, r, u, d, ld);
	drawRD(l, r, u, d, rd);

	g.DrawImage(sprite, luRect,
		(TILE_PIXEL / 2)*cellData[0].x, (TILE_PIXEL / 2)*cellData[0].y,
		TILE_PIXEL / 2, TILE_PIXEL / 2, UnitPixel);
	g.DrawImage(sprite, ruRect,
		(TILE_PIXEL / 2)*cellData[1].x, (TILE_PIXEL / 2)*cellData[1].y,
		TILE_PIXEL / 2, TILE_PIXEL / 2, UnitPixel);
	g.DrawImage(sprite, ldRect,
		(TILE_PIXEL / 2)*cellData[2].x, (TILE_PIXEL / 2)*cellData[2].y,
		TILE_PIXEL / 2, TILE_PIXEL / 2, UnitPixel);
	g.DrawImage(sprite, rdRect,
		(TILE_PIXEL / 2)*cellData[3].x, (TILE_PIXEL / 2)*cellData[3].y,
		TILE_PIXEL / 2, TILE_PIXEL / 2, UnitPixel);
}
