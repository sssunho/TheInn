#pragma once

#include "framework.h"
#include "mapEditor.h"
#include <string>

class Autotile
{

	friend class TilesetEditor;
	friend class MapEditor;

private:
	std::wstring path;
	POINT ID;
	Gdiplus::Image* sprite;
	Autotile() {}

	POINT cellData[4];

	void drawLU(POINT l, POINT r, POINT u, POINT d, POINT lu);
	void drawRU(POINT l, POINT r, POINT u, POINT d, POINT ru);
	void drawLD(POINT l, POINT r, POINT u, POINT d, POINT ld);
	void drawRD(POINT l, POINT r, POINT u, POINT d, POINT rd);

public:
	Autotile(std::wstring name, POINT id) : path(name), ID(id)
	{
		sprite = Gdiplus::Image::FromFile(name.c_str());
	}

	~Autotile() 
	{ 
		delete sprite;
	}

	void draw(Gdiplus::Graphics& g, POINT dest);
	
	void drawIcon(Gdiplus::Graphics& g, POINT dest);

	POINT getID() { return ID; }
};