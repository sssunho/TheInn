#include "framework.h"
#include "headers.h"

using namespace std;
extern RECT clientRect;
POINT Camera::pos = { 0, 0 };
int Camera::margin = 0;

map<string, GameObject*> ObjectManager::objTable = map<string, GameObject*>();
list<GameObject*> ObjectManager::fxList;
priority_queue<GameObject*, vector<GameObject*>, ObjectManager::cmp> ObjectManager::pq;

Actor::Actor(POINT p, POINT v, const string spriteName, DIRECTION dir, int size)
	: GameObject(p, v), dir(DIRECTION::D), spriteName(spriteName)
{
	collider.pos = this->pos;
	collider.owner = this;
	collider.size = size;
	animation.repeat = true;
	setDirection(dir);
}

void Actor::setState(ActorState st)
{
	AnimationManager& am = AnimationManager::getInstance();

	if (st == ActorState::IDLE)
	{
		switch (dir)
		{
		case DIRECTION::D:
			animation = am.getAnimation(spriteName, "standF");
			break;
		case DIRECTION::R:
		case DIRECTION::L:
			animation = am.getAnimation(spriteName, "standS");
			break;
		case DIRECTION::U:
			animation = am.getAnimation(spriteName, "standB");
			break;
		case DIRECTION::RU:
		case DIRECTION::LU:
			animation = am.getAnimation(spriteName, "standBD");
			break;
		case DIRECTION::LD:
		case DIRECTION::RD:
			animation = am.getAnimation(spriteName, "standFD");
			break;
		}
	}

	switch (state)
	{
	case ActorState::IDLE:					// IDLE -> STATE
		if (st == ActorState::ONMOVE)
		{
			switch (dir)
			{
			case DIRECTION::D:
				animation = am.getAnimation(spriteName, "walkF");
				break;
			case DIRECTION::R:
			case DIRECTION::L:
				animation = am.getAnimation(spriteName, "walkS");
				break;
			case DIRECTION::U:
				animation = am.getAnimation(spriteName, "walkB");
				break;
			case DIRECTION::RU:
			case DIRECTION::LU:
				animation = am.getAnimation(spriteName, "walkBD");
				break;
			case DIRECTION::LD:
			case DIRECTION::RD:
				animation = am.getAnimation(spriteName, "walkFD");
				break;
			}
		}
		else if (st == ActorState::VSLASH)
		{
			switch (dir)
			{
			case DIRECTION::D:
				animation = am.getAnimation(spriteName, "vslashF", false);
				break;
			case DIRECTION::R:
			case DIRECTION::L:
				animation = am.getAnimation(spriteName, "vslashS", false);
				break;
			case DIRECTION::U:
				animation = am.getAnimation(spriteName, "vslashB", false);
				break;
			case DIRECTION::RU:
			case DIRECTION::LU:
				animation = am.getAnimation(spriteName, "vslashBD", false);
				break;
			case DIRECTION::LD:
			case DIRECTION::RD:
				animation = am.getAnimation(spriteName, "vslashFD", false);
				break;
			}
		}
		break;

	case ActorState::ONMOVE:
		if (st == ActorState::VSLASH)
		{
			switch (dir)
			{
			case DIRECTION::D:
				animation = am.getAnimation(spriteName, "vslashF", false);
				break;
			case DIRECTION::R:
			case DIRECTION::L:
				animation = am.getAnimation(spriteName, "vslashS", false);
				break;
			case DIRECTION::U:
				animation = am.getAnimation(spriteName, "vslashB", false);
				break;
			case DIRECTION::RU:
			case DIRECTION::LU:
				animation = am.getAnimation(spriteName, "vslashBD", false);
				break;
			case DIRECTION::LD:
			case DIRECTION::RD:
				animation = am.getAnimation(spriteName, "vslashFD", false);
				break;
			}
		}
		break;

	case ActorState::VSLASH:
		if (st == ActorState::HSLASH)
		{
			switch (dir)
			{
			case DIRECTION::D:
				animation = am.getAnimation(spriteName, "hslashF", false);
				break;
			case DIRECTION::R:
			case DIRECTION::L:
				animation = am.getAnimation(spriteName, "hslashS", false);
				break;
			case DIRECTION::U:
				animation = am.getAnimation(spriteName, "hslashB", false);
				break;
			case DIRECTION::RU:
			case DIRECTION::LU:
				animation = am.getAnimation(spriteName, "hslashBD", false);
				break;
			case DIRECTION::LD:
			case DIRECTION::RD:
				animation = am.getAnimation(spriteName, "hslashFD", false);
				break;
			}

		}
		break;

	case ActorState::HSLASH:
		if (st == ActorState::STAB)
		{
			switch (dir)
			{
			case DIRECTION::D:
				animation = am.getAnimation(spriteName, "stabF", false);
				break;
			case DIRECTION::R:
			case DIRECTION::L:
				animation = am.getAnimation(spriteName, "stabS", false);
				break;
			case DIRECTION::U:
				animation = am.getAnimation(spriteName, "stabB", false);
				break;
			case DIRECTION::RU:
			case DIRECTION::LU:
				animation = am.getAnimation(spriteName, "stabBD", false);
				break;
			case DIRECTION::LD:
			case DIRECTION::RD:
				animation = am.getAnimation(spriteName, "stabFD", false);
				break;
			}

		}
		break;
	}
	state = st;
}

void Actor::setDirection(DIRECTION dir)
{
	AnimationManager& am = AnimationManager::getInstance();
	switch (state)
	{
	case ActorState::IDLE:
	{
		switch (dir)
		{
		case DIRECTION::D:
			animation = am.getAnimation(spriteName, "standF");
			break;
		case DIRECTION::R:
		case DIRECTION::L:
			animation = am.getAnimation(spriteName, "standS");
			break;
		case DIRECTION::U:
			animation = am.getAnimation(spriteName, "standB");
			break;
		case DIRECTION::RU:
		case DIRECTION::LU:
			animation = am.getAnimation(spriteName, "standBD");
			break;
		case DIRECTION::LD:
		case DIRECTION::RD:
			animation = am.getAnimation(spriteName, "standFD");
			break;
		}
	}
		break;

	case ActorState::ONMOVE:
	{
		switch (dir)
		{
		case DIRECTION::D:
			animation = am.getAnimation(spriteName, "walkF");
			break;
		case DIRECTION::R:
		case DIRECTION::L:
			animation = am.getAnimation(spriteName, "walkS");
			break;
		case DIRECTION::U:
			animation = am.getAnimation(spriteName, "walkB");
			break;
		case DIRECTION::RU:
		case DIRECTION::LU:
			animation = am.getAnimation(spriteName, "walkBD");
			break;
		case DIRECTION::LD:
		case DIRECTION::RD:
			animation = am.getAnimation(spriteName, "walkFD");
			break;
		}
	}
		break;
	}
	Actor::dir = dir;
}

void Actor::draw(HDC& hdc)
{
	int flag = 
		dir == DIRECTION::R || dir == DIRECTION::RU || dir == DIRECTION::RD ?
		TF_YFLIP : 0;
	POINT drawPoint = pixelToScreen(pos) + spriteOffset;
	animation.draw(hdc, drawPoint.x, drawPoint.y, flag);
}

void Actor::update(float dt)
{
	animation.update();
	VECTOR dest = pos + dt * vel;
	collider.unset();
	collider.pos = dest;
	int flag = collider.collision();
	if (!collider.collision())
		pos = dest;
	collider.update(dt);
}

bool Camera::isIn(POINT p)
{
	POINT rel = p - pos;
	if (rel.x < -clientRect.right / 2 - margin || 
		rel.y < -clientRect.bottom / 2 - margin ||
		rel.x > margin + clientRect.right / 2 ||
		rel.y > margin + clientRect.bottom / 2)
		return false;
	return true;
}

void Camera::Bound(POINT p)
{
	int width = MapManager::loadedMap->getWidth();
	int height = MapManager::loadedMap->getHeight();
	if (p.x - clientRect.right / 2 < 0)
		pos.x = clientRect.right / 2;
	else if (p.x + clientRect.right / 2 > width)
		pos.x = width - clientRect.right / 2;
	else
		pos.x = p.x;

	if (p.y - clientRect.bottom / 2 < 0)
		pos.y = clientRect.bottom / 2;
	else if (p.y + clientRect.bottom / 2 > height)
		pos.y = height - clientRect.bottom / 2;
	else
		pos.y = p.y;
}

bool Collider::collision()
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (MapManager::loadedMap->isBlock(pos + POINT({ i * CELL_PIXEL, j * CELL_PIXEL })))
				return true;
	return false;
}

void Collider::set()
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			MapManager::loadedMap->setBlock(pos + POINT({ i * CELL_PIXEL, j * CELL_PIXEL }));
}

void Collider::unset()
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			MapManager::loadedMap->unsetBlock(pos + POINT({ i * CELL_PIXEL, j * CELL_PIXEL }));
}

void Collider::update(float dt)
{
	pos = owner->pos;
	set();
}

Actor* ObjectManager::createActor(string name, POINT pos, VECTOR vel, 
									   string spriteName, DIRECTION dir, int size)
{
	Actor* instance = new Actor(pos, vel, spriteName, dir, size);
	objTable.insert(pair<string, GameObject*>(name, instance));
	return instance;
}

void ObjectManager::createFX(string spriteName, string aniName, POINT pos, DIRECTION dir)
{
	SpriteFX* instance = new SpriteFX(pos, spriteName, aniName, dir);
	fxList.push_back(instance);
}

Actor * ObjectManager::findActor(std::string name)
{
	Actor* temp;
	if (objTable.find(name) != objTable.end())
		return static_cast<Actor*>(objTable[name]);
	return nullptr;
}

void ObjectManager::draw(HDC& hdc)
{
	while (!pq.empty())
	{
		pq.top()->draw(hdc);
		pq.pop();
	}

}

void ObjectManager::update(float dt)
{
	map<string, GameObject*>::iterator it;
	list<GameObject*>::iterator lit;

	for (it = objTable.begin(); it != objTable.end(); )
	{
		if (it->second->destroy)
		{
			delete it->second;
			it = objTable.erase(it);
		}
		else
			it++;
	}

	for (lit = fxList.begin(); lit != fxList.end(); )
	{
		if ((*lit)->destroy)
		{
			delete (*lit);
			lit = fxList.erase(lit);
		}
		else
		{
			(*lit)->update(dt);
			lit++;
		}
	}

	for (it = objTable.begin(); it != objTable.end(); it++)
	{
		(*it).second->update(dt);
	}

	for (it = objTable.begin(); it != objTable.end(); it++)
	{
		if (Camera::isIn(it->second->pos))
			pq.push(it->second);
	}

	for (lit = fxList.begin(); lit != fxList.end(); )
	{
		if (Camera::isIn((*lit)->pos))
			pq.push(*(lit++));
		else
			lit++;
	}
}

SpriteFX::SpriteFX(POINT p, const string spriteName, const string aniName, DIRECTION dir)
	: Actor(p, { 0, 0 }, spriteName, dir)
{
	animation = AnimationManager::getInstance().getAnimation(spriteName, aniName, false);
}

void SpriteFX::update(float dt)
{
	animation.update();
	if (!animation.isPlaying())
		destroyObj();
}

void SpriteFX::draw(HDC & hdc)
{
	POINT drawPoint = pixelToScreen(pos) + spriteOffset;
	animation.draw(hdc, drawPoint.x, drawPoint.y);
}

