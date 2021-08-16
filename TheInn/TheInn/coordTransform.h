#pragma once
#ifndef __COORDTRANS__
#define __COORDTRANS__

#include "map.h"
#include "gameObject.h"
#include "pointVector.h"
#include "framework.h"

extern RECT clientRect;

#define pixelToScreen(p) (POINT(p) - Camera::pos + POINT({ clientRect.right / 2, clientRect.bottom / 2 }))
#define screenToPixel(p) (POINT(p) + Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }))
#define screenToCell(p) (screenToPixel(p) / CELL_PIXEL)
#define pixelToTile(p) (p / TILE_PIXEL)
#define tileToPixel(p) (p * TILE_PIXEL)
#define pixelToCell(p) (p / CELL_PIXEL)
#define cellToPixel(p) (p * CELL_PIXEL)
#define cellToScreen(p) {pixelToScreen(cellToPixel(p))}

#endif