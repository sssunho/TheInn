#include "mapEditor.h"
#include "tileSetEditor.h"
#include "gridMap.h"
#include "pointVector.h"
#include "wndControl.h"
#include <fstream>
#include <vector>

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
		tsdataFile.close();
		spritePath = L"Graphics/Tilesets/" + spritePath;
		tileWnd = new TilesetEditor(tileset, spritePath);
		mapdataFile >> nx >> ny;

		layer[0] = new Bitmap(nx * TILE_PIXEL, ny* TILE_PIXEL, PixelFormat32bppARGB);
		layer[1] = new Bitmap(nx * TILE_PIXEL, ny* TILE_PIXEL, PixelFormat32bppARGB);
		layer[2] = new Bitmap(nx * TILE_PIXEL, ny* TILE_PIXEL, PixelFormat32bppARGB);

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
				canvas0.DrawImage(tileWnd->getSprite(),
					drawDest,
					start.x*TILE_PIXEL, (start.y - 1)*TILE_PIXEL,
					TILE_PIXEL, TILE_PIXEL,
					UnitPixel
				);
			}

		k = 0;
		for (int i = 0; i < ny; i++)
			for (int j = 0; j < nx; j++)
			{
				int k = 0;
				drawDest.X = j * TILE_PIXEL;
				drawDest.Y = i * TILE_PIXEL;
				mapdataFile >> start.x >> start.y;
				layerData[0][k++] = POINT({ start.x, start.y });
				canvas1.DrawImage(tileWnd->getSprite(),
					drawDest,
					start.x*TILE_PIXEL, (start.y - 1)*TILE_PIXEL,
					TILE_PIXEL, TILE_PIXEL,
					UnitPixel
				);
			}

		k = 0;
		for (int i = 0; i < ny; i++)
			for (int j = 0; j < nx; j++)
			{
				int k = 0;
				drawDest.X = j * TILE_PIXEL;
				drawDest.Y = i * TILE_PIXEL;
				mapdataFile >> start.x >> start.y;
				layerData[0][k++] = POINT({ start.x, start.y });
				canvas2.DrawImage(tileWnd->getSprite(),
					drawDest,
					start.x*TILE_PIXEL, (start.y - 1)*TILE_PIXEL,
					TILE_PIXEL, TILE_PIXEL,
					UnitPixel
				);
			}
		t = clock() - t;
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

void MapEditor::drawTile(POINT mousePos)
{
	POINT srcCoord = tileWnd->getSelectPos();
	Graphics g(layer[selectedLayer]);
	POINT tileCoord = getTileCoord(mousePos);
	Rect dest({ tileCoord.x * TILE_PIXEL, tileCoord.y * TILE_PIXEL, TILE_PIXEL, TILE_PIXEL });
	Region clearRegion(dest);
	Region oldRegion;
	g.GetClip(&oldRegion);
	g.SetClip(&clearRegion);
	g.Clear(Color(0, 255, 255, 255));
	g.SetClip(&oldRegion);
	if(selectedLayer == 0)
		g.FillRectangle(&SolidBrush(Color(255, 255, 255)), dest);

	layerData[selectedLayer][tileCoord.x + tileCoord.y*nx] = srcCoord;

	if (srcCoord == POINT({ 0, 0 }))
		return;

	g.DrawImage(tileWnd->getSprite(),
		dest,
		srcCoord.x * TILE_PIXEL, (srcCoord.y - 1) * TILE_PIXEL, TILE_PIXEL, TILE_PIXEL,
		UnitPixel
	);

}

void MapEditor::setSize(int x, int y)
{
	Bitmap* newLayer[3];

	for (int i = 0; i < 3; i++)
	{
		newLayer[i] = new Bitmap(x*TILE_PIXEL, y*TILE_PIXEL, PixelFormat64bppARGB);
		Graphics g(newLayer[i]);
		g.DrawImage(layer[i], Rect({ 0, 0, (int)layer[i]->GetWidth(), (int)layer[i]->GetHeight() }),
			0, 0, layer[i]->GetWidth(), layer[i]->GetHeight(), UnitPixel);
		delete layer[i];
		layer[i] = newLayer[i];
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
