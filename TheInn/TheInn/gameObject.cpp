#include "gameObject.h"
#include "framework.h"
#include <string>

using namespace std;
extern RECT clientRect;

Actor::Actor(int x, int y, DIRECTION dir, string spriteName)
	: GameObject(x, y), dir(DIRECTION::D), spriteName(spriteName)
{
	AnimationManager& am = AnimationManager::getInstance();
	setDirection(dir);
}

void Actor::setState(ActorState st)
{
	AnimationManager& am = AnimationManager::getInstance();
	switch (state)
	{
	case ActorState::IDLE:
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
		break;

	case ActorState::ONMOVE:
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
			break;
		}
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
	TransformType flag =
		dir == DIRECTION::R || dir == DIRECTION::RU || dir == DIRECTION::RD ?
		FLIP_Y : NONE;
	POINT drawPoint = Camera::toScreenCoord(pos);
	animation.draw(hdc, drawPoint.x, drawPoint.y, flag);
}

void Actor::update(float dt)
{
	animation.update();
}

bool Camera::isIn(POINT p)
{

	return false;
}

POINT Camera::toScreenCoord(POINT p)
{
	Camera& c = Camera::getInstance();
	return p - c.pos + POINT({ clientRect.right / 2, clientRect.bottom / 2 });
}
