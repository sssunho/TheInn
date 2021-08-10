#include "mapEditor.h"
#include "tileSetEditor.h"
#include "gridMap.h"
#include "pointVector.h"
#include "wndControl.h"
#include "autoTile.h"
#include <fstream>
#include <vector>
#include <sstream>

#include <time.h>

using namespace std;
using namespace Gdiplus;



MapEditor::MapEditor(HWND main, HWND tileset, const std::wstring path) :
	hMain(main), hTileset(tileset)
{
	wfstream mapdataFile;
	mapdataFile.open(path);
	if (mapdataFile.is_open())
	{
		wstring spritePath;
		mapdataFile >> tilesetName;
		wfstream tsdataFile;
		tsdataFile.open(L"Data/Tilesets/" + tilesetName);
		tsdataFile >> spritePath;
		spritePath = L"Graphics/Tilesets/" + spritePath;
		tileWnd = new TilesetEditor(tileset, spritePath);
		{
			wstringstream wss;
			WCHAR autoTilePaths[256];
			int stringSize;
			int autotileCount = 0;
			tsdataFile.get();
			tsdataFile.getline(autoTilePaths, 256);
			tsdataFile.getline(autoTilePaths, 256);
			wss << autoTilePaths;
			stringSize = wcslen(autoTilePaths);
			while (wss.tellg() < stringSize - 1)
			{
				wss >> autoTilePaths;
				tileWnd->loadAutoTile(autotileCount++, autoTilePaths);
			}
		}
		tsdataFile.close();
		
		mapdataFile >> nx >> ny;

		layer[0] = new Bitmap(nx * TILE_PIXEL, ny* TILE_PIXEL, PixelFormat32bppARGB);
		layer[1] = new Bitmap(nx * TILE_PIXEL, ny* TILE_PIXEL, PixelFormat32bppARGB);
		layer[2] = new Bitmap(nx * TILE_PIXEL, ny* TILE_PIXEL, PixelFormat32bppARGB);

		canvas[0] = new Graphics(layer[0]);
		canvas[1] = new Graphics(layer[1]);
		canvas[2] = new Graphics(layer[2]);

		Graphics canvas0(layer[0]);
		Graphics canvas1(layer[1]);
		Graphics canvas2(layer[2]);

		layerData[0].resize(nx * ny);
		layerData[1].resize(nx * ny);
		layerData[2].resize(nx * ny);

		POINT start;
		Rect drawDest = { 0,0,TILE_PIXEL,TILE_PIXEL };

		int t = clock();

		int k = 0;
		for(int i = 0 ; i < ny; i++)
			for (int j = 0; j < nx; j++)
			{
				drawDest.X = j * TILE_PIXEL;
				drawDest.Y = i * TILE_PIXEL;
				mapdataFile >> start.x >> start.y;
				layerData[0][k++] = POINT({ start.x, start.y });
				selectedLayer = 0;
				if (start.y == 0)
				{
					if (start.x != 0)
					{
						tileWnd->selectPos = { start.x, 0 };
						drawAutoTile(canvas0, tileWnd->getAutoTile(), POINT({ drawDest.X, drawDest.Y }));
						updateNeighbor(POINT({ drawDest.X, drawDest.Y }));
					}
				}
				else
				{
					canvas0.DrawImage(tileWnd->getSprite(),
						drawDest,
						start.x*TILE_PIXEL, (start.y - 1)*TILE_PIXEL,
						TILE_PIXEL, TILE_PIXEL,
						UnitPixel);

				}
			}
		k = 0;
		for (int i = 0; i < ny; i++)
			for (int j = 0; j < nx; j++)
			{
				drawDest.X = j * TILE_PIXEL;
				drawDest.Y = i * TILE_PIXEL;
				mapdataFile >> start.x >> start.y;
				layerData[1][k++] = POINT({ start.x, start.y });
				selectedLayer = 1;
				if (start.y == 0)
				{
					if (start.x != 0)
					{
						tileWnd->selectPos = { start.x, 0 };
						drawAutoTile(canvas1, tileWnd->getAutoTile(), POINT({ drawDest.X, drawDest.Y }));
						updateNeighbor(POINT({ drawDest.X, drawDest.Y }));
					}
				}
				else
				{
					canvas1.DrawImage(tileWnd->getSprite(),
						drawDest,
						start.x*TILE_PIXEL, (start.y - 1)*TILE_PIXEL,
						TILE_PIXEL, TILE_PIXEL,
						UnitPixel);

				}
			}

		k = 0;
		for (int i = 0; i < ny; i++)
			for (int j = 0; j < nx; j++)
			{
				drawDest.X = j * TILE_PIXEL;
				drawDest.Y = i * TILE_PIXEL;
				mapdataFile >> start.x >> start.y;
				layerData[2][k++] = POINT({ start.x, start.y });
				selectedLayer = 2;
				if (start.y == 0)
				{
					if (start.x != 0)
					{
						tileWnd->selectPos = { start.x, 0 };
						drawAutoTile(canvas2, tileWnd->getAutoTile(), POINT({ drawDest.X, drawDest.Y }));
						updateNeighbor(POINT({ drawDest.X, drawDest.Y }));
					}
				}
				else
				{
					canvas2.DrawImage(tileWnd->getSprite(),
						drawDest,
						start.x*TILE_PIXEL, (start.y - 1)*TILE_PIXEL,
						TILE_PIXEL, TILE_PIXEL,
						UnitPixel);

				}
			}
		selectedLayer = 0;
		mapdataFile.close();
	}
}

void MapEditor::render()
{
	RECT clientRect;
	GetClientRect(hMain, &clientRect);
	
	HDC hdc = GetDC(hMain);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBit = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
	SelectObject(hMemDC, hBit);

	Graphics mainGraphic(hMemDC);
	mainGraphic.FillRectangle(&SolidBrush(Color(255, 255, 255)),
		Rect({ 0, 0, clientRect.right, clientRect.bottom }));
	mainGraphic.DrawImage(layer[0],
		Rect({ 0, 0, clientRect.right, clientRect.bottom }),
		viewPoint.x, viewPoint.y, clientRect.right, clientRect.bottom, UnitPixel);
	mainGraphic.DrawImage(layer[1],
		Rect({ 0, 0, clientRect.right, clientRect.bottom }),
		viewPoint.x, viewPoint.y, clientRect.right, clientRect.bottom, UnitPixel);
	mainGraphic.DrawImage(layer[2],
		Rect({ 0, 0, clientRect.right, clientRect.bottom }),
		viewPoint.x, viewPoint.y, clientRect.right, clientRect.bottom, UnitPixel);
	mainGraphic.DrawRectangle(&Pen(Color(255, 0, 0), 3),
		Rect({ -viewPoint.x, -viewPoint.y, nx*TILE_PIXEL, ny*TILE_PIXEL }));

	BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hMemDC, 0, 0, SRCCOPY);
	
	tileWnd->render();

	DeleteObject(hBit);
	DeleteObject(hMemDC);
	ReleaseDC(hMain, hdc);

}

POINT MapEditor::getTileCoord(POINT mousePos)
{
	return getGridCoord(mousePos + viewPoint);
}

void MapEditor::drawUnitTile(POINT mousePos)
{
	POINT srcCoord = tileWnd->getSelectPos();
	POINT tileCoord = getTileCoord(mousePos);
	Rect dest({ tileCoord.x * TILE_PIXEL, tileCoord.y * TILE_PIXEL, TILE_PIXEL, TILE_PIXEL });

	if (selectedLayer == 0)
	{
		canvas[selectedLayer]->FillRectangle(&SolidBrush(Color(255, 255, 255)), dest);
	}

	if (tileCoord.x < 0 || tileCoord.y < 0 || tileCoord.x >= nx || tileCoord.y >= ny)
		return;

	layerData[selectedLayer][tileCoord.x + tileCoord.y*nx] = srcCoord;

	Gdiplus::Graphics graph(layer[selectedLayer]);
	graph.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
	Gdiplus::SolidBrush transparent(0);
	graph.FillRectangle(&transparent, dest);
	if (srcCoord.y == 0)
	{
		if (srcCoord.x != 0)
		{
			drawAutoTile(*canvas[selectedLayer], tileWnd->getAutoTile(), POINT({ dest.X, dest.Y }));
			updateNeighbor(POINT({dest.X, dest.Y}));
		}
	}
	else
	{
		canvas[selectedLayer]->DrawImage(tileWnd->getSprite(),
			dest,
			srcCoord.x * TILE_PIXEL, (srcCoord.y - 1) * TILE_PIXEL, TILE_PIXEL, TILE_PIXEL,
			UnitPixel);
	}
}

void MapEditor::drawTile(POINT mousePos)
{
	if (tileWnd->isAreaDraw())
	{
		drawArea(mousePos);
	}
	else
	{
		drawUnitTile(mousePos);
	}
}

void MapEditor::drawArea(POINT mousePos)
{
	POINT start = getGridCoord(tileWnd->getAreaStart());
	POINT end = getGridCoord(tileWnd->getAreaEnd());

	for(int i = start.x ; i <= end.x ; i++)
		for (int j = start.y; j <= end.y; j++)
		{
			tileWnd->setSelectPos(start + POINT({ (i - start.x), (j - start.y) }));
			drawUnitTile(mousePos + POINT({ (i - start.x)*TILE_PIXEL, (j - start.y)*TILE_PIXEL }));
		}
}

void MapEditor::setSize(int x, int y)
{
	Bitmap* newLayer[3];

	for (int i = 0; i < 3; i++)
	{
		newLayer[i] = new Bitmap(x*TILE_PIXEL, y*TILE_PIXEL, PixelFormat32bppARGB);
		Graphics g(newLayer[i]);
		g.DrawImage(layer[i], Rect({ 0, 0, (int)layer[i]->GetWidth(), (int)layer[i]->GetHeight() }),
			0, 0, layer[i]->GetWidth(), layer[i]->GetHeight(), UnitPixel);
		delete layer[i];
		delete canvas[i];
		layer[i] = newLayer[i];
		canvas[i] = new Graphics(layer[i]);
		vector<POINT> newLayerData(x * y);

		for (int j = 0; j < layerData[i].size(); j++)
		{
			newLayerData[(j / nx)*x + (j%nx)] = layerData[i][j];
		}

		layerData[i] = newLayerData;
	}
	nx = x; ny = y;
}

void MapEditor::setViewPoint(POINT p)
{
	RECT clientRect;
	GetClientRect(hMain, &clientRect);
	if (layer[0]->GetWidth() < clientRect.right || layer[0]->GetHeight() < clientRect.bottom)
		return;

	if (p.x < 0 || p.y < 0 ||
		p.x + clientRect.right > layer[0]->GetWidth() ||
		p.y + clientRect.bottom > layer[0]->GetHeight())
		return;
	viewPoint = p;
}

void MapEditor::drawArea(POINT p1, POINT p2)
{
	POINT start = { p1.x < p2.x ? p1.x : p2.x, p1.y < p2.y ? p1.y : p2.y };
	POINT end = { p1.x > p2.x ? p1.x : p2.x, p1.y > p2.y ? p1.y : p2.y };
	for (int i = start.x; i <= end.x; i += TILE_PIXEL)
		for (int j = start.y; j <= end.y; j += TILE_PIXEL)
			drawTile(POINT({ i,j }));
}

void MapEditor::drawAutoTile(Graphics& g, Autotile* tile, POINT dest)
{
	Rect luRect = { dest.x, dest.y, TILE_PIXEL / 2, TILE_PIXEL / 2 };
	Rect ruRect = { dest.x + TILE_PIXEL / 2, dest.y, TILE_PIXEL / 2, TILE_PIXEL / 2 };
	Rect ldRect = { dest.x, dest.y + TILE_PIXEL / 2,  TILE_PIXEL / 2, TILE_PIXEL / 2 };
	Rect rdRect = { dest.x + TILE_PIXEL / 2,dest.y + TILE_PIXEL / 2, TILE_PIXEL / 2, TILE_PIXEL / 2 };

	dest = getGridCoord(dest);
	POINT l = getData(dest + POINT({ -1, 0 }));
	POINT r = getData(dest + POINT({ 1, 0 }));
	POINT u = getData(dest + POINT({ 0, -1 }));
	POINT d = getData(dest + POINT({ 0, 1 }));
	POINT lu = getData(dest + POINT({ -1, -1 }));
	POINT ru = getData(dest + POINT({ 1, -1 }));
	POINT ld = getData(dest + POINT({ -1, 1 }));
	POINT rd = getData(dest + POINT({ 1, 1 }));

	tile->drawLU(l, r, u, d, lu);
	tile->drawRU(l, r, u, d, ru);
	tile->drawLD(l, r, u, d, ld);
	tile->drawRD(l, r, u, d, rd);

	g.DrawImage(tile->sprite, luRect, 
		(TILE_PIXEL / 2)*tile->cellData[0].x, (TILE_PIXEL / 2)*tile->cellData[0].y,
		TILE_PIXEL / 2, TILE_PIXEL / 2, UnitPixel);
	g.DrawImage(tile->sprite, ruRect, 
		(TILE_PIXEL / 2)*tile->cellData[1].x, (TILE_PIXEL / 2)*tile->cellData[1].y,
		TILE_PIXEL / 2, TILE_PIXEL / 2, UnitPixel);
	g.DrawImage(tile->sprite, ldRect, 
		(TILE_PIXEL / 2)*tile->cellData[2].x, (TILE_PIXEL / 2)*tile->cellData[2].y,
		TILE_PIXEL / 2, TILE_PIXEL / 2, UnitPixel);
	g.DrawImage(tile->sprite, rdRect, 
		(TILE_PIXEL / 2)*tile->cellData[3].x, (TILE_PIXEL / 2)*tile->cellData[3].y,
		TILE_PIXEL / 2, TILE_PIXEL / 2, UnitPixel);
}

void MapEditor::updateNeighbor(POINT dest)
{

	POINT pDest = getGridCoord(dest);
	POINT ID = tileWnd->getAutoTile()->getID();
	POINT l = getData(pDest + POINT({ -1, 0 }));
	POINT r = getData(pDest + POINT({ 1, 0 }));
	POINT u = getData(pDest + POINT({ 0, -1 }));
	POINT d = getData(pDest + POINT({ 0, 1 }));
	POINT lu = getData(pDest + POINT({ -1, -1 }));
	POINT ru = getData(pDest + POINT({ 1, -1 }));
	POINT ld = getData(pDest + POINT({ -1, 1 }));
	POINT rd = getData(pDest + POINT({ 1, 1 }));

	if (ID == l)
		drawAutoTile(*canvas[selectedLayer], tileWnd->getAutoTile(),
			dest - POINT({ TILE_PIXEL, 0 }));
	if (ID == r)
		drawAutoTile(*canvas[selectedLayer], tileWnd->getAutoTile(),
			dest + POINT({ TILE_PIXEL, 0 }));
	if (ID == u)
		drawAutoTile(*canvas[selectedLayer], tileWnd->getAutoTile(),
			dest - POINT({ 0, TILE_PIXEL }));
	if (ID == d)
		drawAutoTile(*canvas[selectedLayer], tileWnd->getAutoTile(),
			dest + POINT({ 0, TILE_PIXEL }));
	if (ID == lu)
		drawAutoTile(*canvas[selectedLayer], tileWnd->getAutoTile(),
			dest + POINT({ -TILE_PIXEL, -TILE_PIXEL }));
	if (ID == ru)
		drawAutoTile(*canvas[selectedLayer], tileWnd->getAutoTile(),
			dest + POINT({ TILE_PIXEL, -TILE_PIXEL }));
	if (ID == ld)
		drawAutoTile(*canvas[selectedLayer], tileWnd->getAutoTile(),
			dest + POINT({ -TILE_PIXEL, TILE_PIXEL }));
	if (ID == rd)
		drawAutoTile(*canvas[selectedLayer], tileWnd->getAutoTile(),
			dest + POINT({ TILE_PIXEL, TILE_PIXEL }));

}

void MapEditor::save()
{
	wstring path = saveFile(hMain, L"mapdata");
	if (path == L"")
		return;

	wfstream file;
	file.open(path, ios::out);
	if (file.is_open())
	{
		file << tilesetName << L"\n" << nx << L' ' << ny << L'\n';

		for (int i = 0; i < nx*ny; i++)
		{
			file << layerData[0][i].x << L' ' << layerData[0][i].y << L' ';
			if (i % nx == nx - 1)
				file << L'\n';
		}
		for (int i = 0; i < nx*ny; i++)
		{
			file << layerData[1][i].x << L' ' << layerData[1][i].y << L' ';
			if (i % nx == nx - 1)
				file << L'\n';
		}
		for (int i = 0; i < nx*ny; i++)
		{
			file << layerData[2][i].x << L' ' << layerData[2][i].y << L' ';
			if (i % nx == nx - 1)
				file << L'\n';
		}

		file.close();
	}
}

POINT MapEditor::getData(POINT p)
{
	if (p.x >= nx || p.y >= ny || p.x < 0 || p.y < 0)
		return { -1,-1 };
	return layerData[selectedLayer][p.x + p.y * nx];
}
