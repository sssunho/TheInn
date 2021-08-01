#include "gridMap.h"
#include "framework.h"
#include "mapEditor.h"
#include "ui.h"
#include "pointVector.h"
#include <fstream>
#include <map>
#include <string>
#include <commdlg.h>

using namespace std;
using namespace Gdiplus;

map<HWND, TileSetEditor*> tileSetEditorMap;

void loadTileSet(HWND hwnd, HWND hwndClient)
{
	OPENFILENAME ofn;
	wfstream tileSetDataFile;
	WCHAR path[512] = L"";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = L"png(*.png*)\0*.png\0";
	ofn.lpstrFile = path;
	ofn.nMaxFile = 512;
	ofn.lpstrInitialDir = L"c:\\";
	if (GetOpenFileName(&ofn) == 0)
		return;
	int i, j, k;
	WCHAR fileName[32];
	for (i = wcslen(path) - 1; i >= 0 && path[i] != L'\\'; i--);
	for (j = wcslen(path) - 1; j >= 0 && path[j] != L'.'; j--);
	for (k = 0; k < 31 && i < j - 1; k++)
		fileName[k] = path[++i];
	fileName[k] = '\0';
	wstring fileNameString = fileName;
	tileSetDataFile.open(fileNameString + L".txt", ios::in);
	if (tileSetDataFile.is_open())
	{
		tileSetDataFile.close();
	}
	else
	{
		tileSetDataFile.open(fileName, ios::out);
		tileSetDataFile.close();
	}
/*
	if (tileSetMap.find(fileNameString) != tileSetMap.end())
	{
		tileSetMap.insert(pair<wstring, HWND>(fileNameString, CreateMDIChild(hwndClient, L"Tile Window")));
	}*/

	//Image* loadedTile = Image::FromFile(path);

	HWND newTileWindow = CreateMDIChild(hwndClient, L"Tile Window");
	tileSetEditorMap.insert(pair<HWND, TileSetEditor*>(newTileWindow,
		new TileSetEditor(newTileWindow, Image::FromFile(path), false)));

}

TileSetEditor::TileSetEditor(HWND hWnd, Image* image, bool animation) 
	: hTileWnd(hWnd), tileSetImage(image), animation(animation), selectedTile(POINT({-1, -1})), mousePos(POINT({ -1, -1 }))
{
	int nx; int ny;
 	nx = image->GetWidth() / TILE_DIM_PIXEL;
	ny = image->GetHeight() / TILE_DIM_PIXEL;
	int width = image->GetWidth();
	int height = image->GetHeight();

	SetClientSize(hWnd, width, height);

	HDC hdc = GetDC(hWnd);
	renderTileSet(hdc);
	ReleaseDC(hWnd, hdc);
}

void TileSetEditor::renderTileSet(HDC& hdc)
{
	if (tileSetImage == NULL)
		return;
	Graphics g(hdc);
	Rect rect(0, 0, tileSetImage->GetWidth(), tileSetImage->GetHeight());
	SolidBrush brush(Color(255, 255, 255));
	g.FillRectangle(&brush, rect);
	g.DrawImage(tileSetImage, 0, 0);
	int nx = tileSetImage->GetWidth() / TILE_DIM_PIXEL;
	int ny = tileSetImage->GetHeight() / TILE_DIM_PIXEL;

	for(int i = 0 ; i < ny ; i++)
		for (int j = 0; j < nx; j++)
		{
			Pen pen(Color(0, 0, 0), 1);
			Rect rect = { j*TILE_DIM_PIXEL, i*TILE_DIM_PIXEL, TILE_DIM_PIXEL, TILE_DIM_PIXEL };
			g.DrawRectangle(&pen, rect);
		}

	if (selectedTile.x >= 0)
	{
		Pen pen(Color(0, 100, 200), 3);
		Rect rect = { selectedTile.x*TILE_DIM_PIXEL, selectedTile.y*TILE_DIM_PIXEL, TILE_DIM_PIXEL, TILE_DIM_PIXEL };
		g.DrawRectangle(&pen, rect);
	}
}

void TileSetEditor::renderSelection(HDC& hdc, POINT pos)
{
	if (toTileCoord(pos) == mousePos)
		return;
	mousePos = toTileCoord(pos);
	renderTileSet(hdc);
	pos = toTileCoord(pos);
	Pen pen(Color(255, 0, 0), 3);
	Rect rect = { pos.x*TILE_DIM_PIXEL, pos.y*TILE_DIM_PIXEL, TILE_DIM_PIXEL, TILE_DIM_PIXEL };
	Graphics g(hdc); g.DrawRectangle(&pen, rect);
}

void TileSetEditor::selectTile(POINT pos)
{
	selectedTile = toTileCoord(pos);
}

void TileSetEditor::setMousePos(POINT pos)
{
	mousePos = toTileCoord(pos);
}

void setTileDataDefault()
{
	
}

void createTileSetData(const WCHAR* fileName)
{
	wfstream mapDataFile;
	mapDataFile.open(fileName, ios::out);
	
	mapDataFile.close();
}

POINT toTileCoord(POINT p)
{
	POINT pp = { p.x / TILE_DIM_PIXEL, p.y / TILE_DIM_PIXEL };
	return POINT({ p.x / TILE_DIM_PIXEL, p.y / TILE_DIM_PIXEL });
}