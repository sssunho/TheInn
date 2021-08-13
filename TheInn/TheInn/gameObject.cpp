#include "framework.h"
#include "headers.h"

using namespace std;
extern RECT clientRect;
POINT Camera::pos = { 0, 0 };
int Camera::margin = 0;

extern HDC hMainDC;

map<string, GameObject*> ObjectManager::objTable = map<string, GameObject*>();
list<GameObject*> ObjectManager::objList;
priority_queue<GameObject*, vector<GameObject*>, ObjectManager::cmp> ObjectManager::pq;

Actor::Actor(POINT p, POINT v, const string spriteName, DIRECTION dir, int size)
	: GameObject(p, v), dir(DIRECTION::D), spriteName(spriteName)
{
	collider.pos = this->pos;
	collider.owner = this;
	collider.size = size;
	animation.repeat = true;
	Sprite sprite = SpriteManager::getInstance().getSprite(spriteName, "standF");
	int width = sprite.getWidth();
	int height = sprite.getHeight();
	spriteOffset = { (size / 2)*CELL_PIXEL, height / 2 - (size / 2)*CELL_PIXEL };
}

void Actor::setAnimation(string name, bool repeat)
{
	animation = AnimationManager::getInstance().getAnimation(spriteName, name, repeat);
}

void Actor::setDirection(DIRECTION dir)
{
	Actor::dir = dir;
}

void Actor::setSpeed(float f)
{
	switch (dir)
	{
	case DIRECTION::L:
		vel = { -f, 0 };
		break;
	case DIRECTION::R:
		vel = { f, 0 };
		break;
	case DIRECTION::U:
		vel = { 0, -f };
		break;
	case DIRECTION::D:
		vel = { 0, f };
		break;
	case DIRECTION::LU:
		vel = { -f / 1.4f, -f / 1.4f };
		break;
	case DIRECTION::RU:
		vel = { f / 1.4f, -f / 1.4f };
		break;
	case DIRECTION::LD:
		vel = { -f / 1.4f, f / 1.4f };
		break;
	case DIRECTION::RD:
		vel = { f / 1.4f, f / 1.4f };
		break;
	}
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
	VECTOR ds = dt * vel;
	VECTOR remain = { abs(ds.x), abs(ds.y) };
	collider.unset();
	while (remain.x > 0)
	{
		float unit = abs(ds.x) > CELL_PIXEL ? (ds.x > 0 ? 1 : -1) * CELL_PIXEL : ds.x;
		collider.pos.x += unit;
		ds.x -= unit;
		remain.x -= abs(unit);
		if (collider.collision())
			break;
		pos.x = collider.pos.x;
	}
	while (remain.y > 0)
	{
		float unit = abs(ds.y) > CELL_PIXEL ? (ds.y > 0 ? 1 : -1) * CELL_PIXEL : ds.y;
		collider.pos.y += unit;
		ds.y -= unit;
		remain.y -= abs(unit);
		if (collider.collision())
			break;
		pos.y = collider.pos.y;
	}
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
	instance->setAnimation("standF");
	objTable.insert(pair<string, GameObject*>(name, instance));
	objList.push_back(instance);
	return instance;
}

void ObjectManager::createFX(string spriteName, string aniName, POINT pos, int flag)
{
	SpriteFX* instance = new SpriteFX(pos, spriteName, aniName, flag);
	objList.push_back(instance);
	pq.push(instance);
}

Actor * ObjectManager::findActor(std::string name)
{
	Actor* temp;
	if (objTable.find(name) != objTable.end())
		return static_cast<Actor*>(objTable[name]);
	return nullptr;
}

void ObjectManager::registerObj(string name, GameObject * obj)
{
	objTable.insert(pair<string, GameObject*>(name, obj));
	objList.push_back(obj);
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

	for (lit = objList.begin(); lit != objList.end(); )
	{
		if ((*lit)->destroy)
		{
			delete (*lit);
			lit = objList.erase(lit);
		}
		else
		{
			if ((*lit)->mState)
			{
				(*lit)->mState->update(*lit, dt);
			}
			else
			{
				(*lit)->update(dt);
			}
			lit++;
		}
	}

	for (lit = objList.begin(); lit != objList.end(); )
	{
		if (Camera::isIn((*lit)->pos))
			pq.push(*(lit++));
		else
			lit++;
	}
}

void ObjectManager::forEachInArea(Area * area, int param, void(*func)(GameObject *, int))
{
	list<GameObject*>::iterator it;
	for (it = objList.begin(); it != objList.end(); it++)
	{
		if(area->isIn((*it)->pos))
			func(*it, param);
	}
}

SpriteFX::SpriteFX(POINT p, const string spriteName, const string aniName, int flag)
	: GameObject(p, { 0, 0 }), flag(flag)
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
	POINT drawPoint = pixelToScreen(pos);
	animation.draw(hdc, drawPoint.x, drawPoint.y, flag);
}

void GameObject::setState(StateMachine * state)
{
	if (StateMachine* newState = mState->setState(this, state))
	{
		mState = newState;
	}
}

void damageTo(GameObject* target, int damage)
{
	Enemy* enemy;
	if(enemy = dynamic_cast<Enemy*>(target))
	{
		enemy->HP -= damage;
		enemy->setState(new HIT);
	}
}