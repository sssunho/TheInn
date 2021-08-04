#include "gridMap.h"
#include "framework.h"

POINT getGridCoord(POINT pos)
{
	return POINT({ pos.x / TILE_PIXEL , pos.y / TILE_PIXEL });
}
