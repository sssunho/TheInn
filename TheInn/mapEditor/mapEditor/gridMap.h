#pragma once
#ifndef __GRIDMAP__
#define __GRIDMAP__

#include "framework.h"

#define CELL_PIXEL 8
#define TILE_CELL 4
#define TILE_PIXEL (CELL_PIXEL * TILE_CELL)

POINT getGridCoord(POINT pos);

#endif