#include "node.h"
#include "headers.h"
#include <Bits.h>

stack<POINT> path;
vector<vector<bool>> visit;
vector<vector<bool>> close;
const Map* loadedMap;

struct compare
{
	bool operator()(Node* p1, Node* p2)
	{
		return p1->getF() == p2->getF() ? p1->getG() > p2->getG() : p1->getF() > p2->getF();
	}
};


priority_queue<Node*, vector<Node*>, compare> openNodes;

Node* findPath(POINT start, POINT end, int size)
{
	TIMER limitTimer;
	loadedMap = MapManager::loadedMap; 
	const vector<vector<Node*>>& graph = loadedMap->graph;
	int i = 0;
	visit = vector<vector<bool>>(loadedMap->getHeight() / CELL_PIXEL, 
				   vector<bool>(loadedMap->getWidth() / CELL_PIXEL, false));
	close = vector<vector<bool>>(loadedMap->getHeight() / CELL_PIXEL,
				   vector<bool>(loadedMap->getWidth() / CELL_PIXEL, false));
	openNodes = priority_queue<Node*, vector<Node*>, compare>();
	graph[start.y][start.x]->setG(0);
	graph[start.y][start.x]->setH(getDistance(start, end));
	graph[start.y][start.x]->setPrev(NULL);

	Node* nearest = graph[start.y][start.x];
	selectNode(start, end, size);

	while (!openNodes.empty())
	{
		Node* temp = openNodes.top();
		openNodes.pop();
		selectNode(temp->getPos(), end, size);
		if (nearest->getH() > temp->getH())
			nearest = temp;
		if (visit[end.y][end.x])
		{
			return graph[end.y][end.x];
		}
		if (limitTimer.getElapsedTime() > 8)
			break;
	}
	return nearest;
}

void selectNode(POINT pos, POINT end, int size)
{
	close[pos.y][pos.x] = true;

	updateNode(POINT({ pos.x - 1, pos.y }), end, pos, size);
	updateNode(POINT({ pos.x + 1, pos.y }), end, pos, size);
	updateNode(POINT({ pos.x, pos.y - 1 }), end, pos, size);
	updateNode(POINT({ pos.x, pos.y + 1 }), end, pos, size);
	updateNode(POINT({ pos.x - 1, pos.y - 1 }), end, pos, size);
	updateNode(POINT({ pos.x - 1, pos.y + 1 }), end, pos, size);
	updateNode(POINT({ pos.x + 1, pos.y - 1 }), end, pos, size);
	updateNode(POINT({ pos.x + 1, pos.y + 1 }), end, pos, size);
}

void updateNode(POINT pos, POINT end, POINT prev, int size)
{
	const vector<vector<Node*>>& graph = loadedMap->graph;
	if (pos.x < 0 || pos.y < 0 || pos.x >= loadedMap->cellNx || pos.y >= loadedMap->cellNy)
		return;
	if (close[pos.y][pos.x] || loadedMap->isBlock(pos, size))
		return;

	Node temp(pos, getDistance(pos, prev) + graph[prev.y][prev.x]->getG(), 
				   getDistance(pos, end), graph[prev.y][prev.x]);

	if (visit[pos.y][pos.x])
	{
		if (temp > *graph[pos.y][pos.x])
		{
			*graph[pos.y][pos.x] = temp;
		}
	}
	else
	{
		visit[pos.y][pos.x] = true;
		*graph[pos.y][pos.x] = temp;
		openNodes.push(graph[pos.y][pos.x]);
	}

}

int getDistance(POINT p1, POINT p2)
{
	int bx = abs(p1.x - p2.x);
	int by = abs(p1.y - p2.y);
	return bx > by ? by * 14 + (bx - by) * 10 : bx * 14 + (by - bx) * 10;
}

void sortAgain()
{
	priority_queue<Node*, vector<Node*>, compare> temp;
	while (!openNodes.empty())
	{
		temp.push(openNodes.top());
		openNodes.pop();
	}
	openNodes = temp;
	
}

void drawPath(HDC& hdc, Node* path)
{
	Graphics g(hdc);
	while (path)
	{
		VECTOR pos = { (float)path->getPos().x , (float)path->getPos().y };
		pos = 8 * pos;
		POINT p = pixelToScreen(pos);
		g.DrawEllipse(&Pen(Color(255, 0, 0), 3), Rect(p.x - 10, p.y - 10, 20, 20));
		path = path->getPrev();
	}
}

void getPath(stack<POINT>& path, POINT start, Node* node)
{
	while (node)
	{
		path.push(node->getPos());
		node = node->getPrev();
	}
}