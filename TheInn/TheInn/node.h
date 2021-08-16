#pragma once
#include "framework.h"

class Node
{
private:
	POINT pos;
	int g;
	int h;
	Node* prev;

public:
	Node(POINT pos = { 0, 0 }, int g = 0, int h = 0, Node* prev = NULL) : pos(pos), g(g), h(h), prev(prev) {}
	int getG() { return g; }
	int getH() { return h; }
	int getF() { return g + h; }
	POINT getPos() { return pos; }
	Node* getPrev() { return prev; }
	void setG(int _g) { g = _g; }
	void setH(int _h) { h = _h; }
	void setPrev(Node* node) { prev = node; }

	bool operator<(Node p)
	{
		return g + h == p.g + p.h ? g > p.g: g + h > p.g + p.h;
	}

	bool operator>(Node p)
	{
		return g + h == p.g + p.h ? g < p.g : g + h < p.g + p.h;
	}
};
