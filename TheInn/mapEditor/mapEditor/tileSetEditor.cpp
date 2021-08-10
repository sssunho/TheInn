#include "tileSetEditor.h"
#include "wndControl.h"
#include "gridMap.h"
#include "pointVector.h"
#include "autoTile.h"
#include <commdlg.h>
#include <fstream>
#include <string>

using namespace std;
using namespace Gdiplus;

TilesetEditor::TilesetEditor(HWND owner, std::wstring path) : hWndOwner(owner)
{
	int i;
	sprite = Image::FromFile(path.c_str());
	SetClientSize(owner, sprite->GetWidth(), sprite->GetHeight() + TILE_PIXEL);
	currentPos = { -1, -1 };
	selectPos = { 0, 0 };

	grid = new Bitmap(sprite->GetWidth(), sprite->GetHeight() + TILE_PIXEL, PixelFormat64bppARGB);
	Graphics g(grid);

	int nx = grid->GetWidth() / TILE_PIXEL;
	int ny = grid->GetHeight() / TILE_PIXEL;

	for (int i = 0; i <= ny; i++)
	{
		Pen pen(Color(0, 0, 0), 1);
		g.DrawLine(&pen, Point({ 0, i*TILE_PIXEL }), Point({ TILE_PIXEL*(nx + 1), i*TILE_PIXEL }));
	}

	for (int i = 0; i <= nx; i++)
	{
		Pen pen(Color(0, 0, 0), 1);
		g.DrawLine(&pen, Point({ i*TILE_PIXEL, 0 }), Point({ i*TILE_PIXEL, TILE_PIXEL*(ny + 2) }));
	}

	tileData.resize(ny);
	for (int i = 0; i < tileData.size(); i++)
		tileData[i].resize(nx, 0);

	for (i = path.size() - 1; i >= 0 && path[i] != L'\\'; i--);
	
	spriteName = path.c_str() + i + 1;

	render();
}

TilesetEditor::~TilesetEditor()
{
	for (int i = 0; i < 5; i++)
		if (!autoTile[i])
			delete autoTile[i];
	delete sprite;
	delete grid;
}

void TilesetEditor::setCurrentPos(POINT mousePos)
{
	POINT p = getGridCoord(mousePos);
	if (p == currentPos)
		return;
	currentPos = p;
	render();
}


void TilesetEditor::render()
{
	Graphics g(hWndOwner);
	RECT clientRect;
	GetClientRect(hWndOwner, &clientRect);


	g.FillRectangle(&SolidBrush(Color(255, 255, 255)),
		Rect({ 0, 0, clientRect.right, clientRect.bottom + TILE_PIXEL }));

	g.DrawImage(sprite, Rect(0, TILE_PIXEL, sprite->GetWidth(), sprite->GetHeight()),
		0, 0, sprite->GetWidth(), sprite->GetHeight(), UnitPixel);
	g.DrawImage(grid, Rect(0, 0, grid->GetWidth(), grid->GetHeight()),
		0, 0, grid->GetWidth(), grid->GetHeight(), UnitPixel);

	if (currentPos.x >= 0 && currentPos.y >= 0)
	{
		Pen pen(Color(255, 0, 0), 3);
		Rect dest = { currentPos.x * TILE_PIXEL, currentPos.y * TILE_PIXEL, TILE_PIXEL, TILE_PIXEL };
		g.DrawRectangle(&pen, dest);
	}

	if (selectPos.x >= 0 && selectPos.y >= 0)
	{
		if (!dragArea)
		{
			Pen pen(Color(0, 0, 255), 3);
			Rect dest = { selectPos.x * TILE_PIXEL, selectPos.y * TILE_PIXEL, TILE_PIXEL, TILE_PIXEL };
			g.DrawRectangle(&pen, dest);
		}
		else
		{
			Pen pen(Color(0, 0, 255), 3);
			POINT startGrid = getGridCoord(dragStart);
			POINT endGrid = getGridCoord(dragEnd);
			Rect dest = { startGrid.x * TILE_PIXEL, startGrid.y * TILE_PIXEL,
							(endGrid.x - startGrid.x + 1) * TILE_PIXEL,
							(endGrid.y - startGrid.y + 1) * TILE_PIXEL };
			g.DrawRectangle(&pen, dest);
		}
	}

	for (int i = 0; i < 7; i++)
	{
		if (autoTile[i])
			autoTile[i]->drawIcon(g, POINT({ 32 * (i + 1), 0 }));
	}
}

void TilesetEditor::drawTile(HWND hWnd)
{
	Graphics g(hWnd);
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	g.DrawImage(sprite, Rect({ 0, 0, clientRect.right, clientRect.bottom }),
		currentPos.x * TILE_PIXEL, (currentPos.y - 1) * TILE_PIXEL,
		TILE_PIXEL, TILE_PIXEL, UnitPixel);
}

void TilesetEditor::drawTileInfo(HWND hWnd, POINT p)
{
	Graphics g(hWnd);
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	int offsetX = clientRect.right / 4;
	int offsetY = clientRect.bottom / 4;

	g.DrawImage(sprite, Rect({ 0, 0, clientRect.right, clientRect.bottom }),
		p.x * TILE_PIXEL, (p.y - 1) * TILE_PIXEL,
		TILE_PIXEL, TILE_PIXEL, UnitPixel);

	for (int i = 0; i < clientRect.right / TILE_CELL; i++)
	{
		g.DrawLine(&Pen(Color(0, 0, 0), 1), 
			Point({ i* offsetX, 0 }), 
			Point({ i* offsetX, clientRect.bottom }));

		g.DrawLine(&Pen(Color(0, 0, 0), 1),
			Point({ 0, i* offsetY }),
			Point({ clientRect.right, i* offsetY }));
	}

	for(int i = 0 ; i < 4 ; i++)
		for (int j = 0; j < 4; j++)
		{
			HDC hdc = GetDC(hWnd);

			if (tileData[p.y][p.x] & mapDataMask[i][j])
				DrawText(hdc, L"x", wcslen(L"x"),
					&RECT({ j * offsetX, i * offsetY, (j + 1)*offsetX, (i + 1)*offsetY }),
					DT_CENTER | DT_VCENTER);
			else
				DrawText(hdc, L"o", wcslen(L"o"),
					&RECT({ j * offsetX, i * offsetY, (j + 1)*offsetX, (i + 1)*offsetY }),
					DT_CENTER | DT_VCENTER);

			ReleaseDC(hWnd, hdc);
		}

}

void TilesetEditor::setTileData(POINT targetTile, POINT targetData)
{
	if (targetTile.x < 0 || targetTile.y < 0 || targetData.x < 0 || targetData.y < 0)
		return;
	tileData[targetTile.y][targetTile.x] =
		tileData[targetTile.y][targetTile.x] ^ mapDataMask[targetData.y][targetData.x];
}

void TilesetEditor::selectTile(POINT p1, POINT p2)
{
	POINT start = { p1.x < p2.x ? p1.x : p2.x, p1.y < p2.y ? p1.y : p2.y };
	POINT end = { p1.x > p2.x ? p1.x : p2.x, p1.y > p2.y ? p1.y : p2.y };

	if (getGridCoord(start) == getGridCoord(end))
	{
		selectTile(start);
		dragArea = false;
		return;
	}
	else
	{
		selectTile(start);
		dragStart = start;
		dragEnd = end;
		dragArea = true;
	}
}

wstring TilesetEditor::getAutotilePath(int i)
{
	if (i > 7)
		return L"";
	if (autoTile[i] == NULL)
		return L"";
	return autoTile[i]->path;
}

void TilesetEditor::loadAutoTile(int i, std::wstring path)
{
	if (i > 7)
		return;
	if (autoTile[i])
		if (autoTile[i]->path == path)
			return;
	if (autoTile[i])
		delete autoTile[i];
	if (path != L"")
	{
		autoTile[i] = new Autotile(path, POINT({ i + 1, 0 }));
	}
	else
	{
		autoTile[i] = nullptr;
	}

}

void TilesetEditor::selectTile(POINT mousePos)
{
	POINT p = getGridCoord(mousePos);
	if (p == selectPos)
		return;
	selectPos = p;
}

void TilesetEditor::save()
{
	wfstream file;
	file.open(L"Data/Tilesets/" + spriteName + L".tsdata", ios::out);
	if (file.is_open())
	{
		file << spriteName << L"\n";
		file << tileData[0].size() << ' ' << tileData.size() << '\n';
		for (int i = 0; i < 7; i++)
		{
			if(autoTile[i])
				file << autoTile[i]->path << ' ';
		}
		file << '\n';
		for (int i = 0; i < tileData.size(); i++)
		{
			for (int j = 0; j < tileData[0].size(); j++)
			{
				file << tileData[i][j] << L' ';
			}
			file << L"\n";
		}
	}

	file.close();
}

