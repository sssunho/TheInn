#pragma once

#include "framework.h"
#include <string>

class MapManager;

class Autotile
{
	friend class MapManager;

private:
	std::string name;
	POINT ID;
	POINT cellData[4];

	void drawLU(POINT l, POINT r, POINT u, POINT d, POINT lu);
	void drawRU(POINT l, POINT r, POINT u, POINT d, POINT ru);
	void drawLD(POINT l, POINT r, POINT u, POINT d, POINT ld);
	void drawRD(POINT l, POINT r, POINT u, POINT d, POINT rd);

public:
	Autotile() : ID(), cellData() {}

	Autotile(std::wstring name, POINT id) : ID(id)
	{
	}

	~Autotile() 
	{ 
	}

	void draw(Gdiplus::Graphics& g, int layer, Gdiplus::Rect& dest, std::string mapName);

	POINT getID() { return ID; }
};