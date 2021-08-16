#ifndef __PATHFIND__
#define __PATHFIND__

#include "framework.h"
#include <stack>
#include <queue>
#include <vector>

using std::stack;
using std::vector;
using std::priority_queue;

class Node;

Node* findPath(POINT start, POINT end, int size = 1);

void selectNode(POINT pos, POINT end, int size);

void updateNode(POINT pos, POINT end, POINT prev, int size);

int getDistance(POINT p1, POINT p2);

void sortAgain();

void drawPath(HDC & hdc, Node * path);

void getPath(stack<POINT>& path, POINT start, Node * node);


#endif