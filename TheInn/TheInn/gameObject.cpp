#include "gameObject.h"
#include "framework.h"
#include <string>

using namespace std;

Actor::Actor(int x, int y, DIRECTION dir, string aniName)
	: GameObject(x, y), dir(DIRECTION::D), animation(NULL)
{
	AnimationManager& am = AnimationManager::getInstance();
	animation = am.getAnimation(aniName);
}

void Actor::setState(ActorState st)
{
	switch (state)
	{
	case ActorState::IDLE:
		if (st == ActorState::ONMOVE)
		{
			sprite.play();
		}
		break;

	case ActorState::ONMOVE:
		if (st == ActorState::IDLE)
		{
			sprite.pause();
			sprite.setFrame(1);
		}
		break;
	}

	state = st;
}

void Actor::setDirection(DIRECTION dir)
{
	switch (dir)
	{
	case DIRECTION::D:
		sprite.setYOffset(0);
		break;
	case DIRECTION::L:
		sprite.setYOffset(1);
		break;
	case DIRECTION::R:
		sprite.setYOffset(2);
		break;
	case DIRECTION::U:
		sprite.setYOffset(3);
		break;
	case DIRECTION::LU:
		sprite.setYOffset(4);
		break;
	case DIRECTION::RU:
		sprite.setYOffset(5);
		break;
	case DIRECTION::LD:
		sprite.setYOffset(6);
		break;
	case DIRECTION::RD:
		sprite.setYOffset(7);
		break;
	}

	Actor::dir = dir;
}