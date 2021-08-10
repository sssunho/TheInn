#pragma once
#ifndef __COORDTRANS__
#define __COORDTRANS__

#include "map.h"
#include "gameObject.h"
#include "pointVector.h"
#include "framework.h"


#define pixelToScreen(p) (p - Camera::pos + POINT({ clientRect.right / 2, clientRect.bottom / 2 }))
#define screenToPixel(p) (p + Camera::pos - POINT({ clientRect.right / 2, clientRect.bottom / 2 }))
#define screenToCell(p) (screenToPixel(p) / CELL_PIXEL)
#define pixelToTile(p) (p / TILE_PIXEL)
#define tileToPixel(p) (TILE_PIXEL / p)
#define pixelToCell(p) (p / CELL_PIXEL)

#endif