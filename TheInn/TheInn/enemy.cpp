#include "enemy.h"

Enemy::Enemy(POINT p, POINT v, const string spriteName, DIRECTION dir, int size)
	: Actor(p, v, spriteName, dir, size) 
{
	mState = new EnemyIDLE;
	mState->enter(this);
	ObjectManager::registerObj("enemy", this);	
}

StateMachine * EnemyIDLE::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<HIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void EnemyIDLE::update(GameObject * obj, float dt)
{
	obj->update(dt);
}

void EnemyIDLE::enter(GameObject * obj)
{
	if (Actor* actor = dynamic_cast<Actor*>(obj))
	{
		switch (actor->getDirection())
		{
		case DIRECTION::L:
			actor->setAnimation("stand4dL");
			break;
		case DIRECTION::R:
			actor->setAnimation("stand4dR");
			break;
		case DIRECTION::U:
			actor->setAnimation("stand4dB");
			break;
		case DIRECTION::D:
			actor->setAnimation("stand4dF");
			break;
		}
	}
}

StateMachine * HIT::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<EnemyIDLE*>(state))
	{
		dynamic_cast<Enemy*>(obj)->setSprite(oldSprite);
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void HIT::update(GameObject * obj, float dt)
{
	obj->update(dt);
	if (timer.getElapsedTime() > 150)
	{
		obj->setState(new EnemyIDLE);
	}
}

void HIT::enter(GameObject * obj)
{
	timer.set();
	if (Enemy* actor = dynamic_cast<Enemy*>(obj))
	{
		actor->HP -= damage;
		oldSprite = actor->getSpriteName();
		actor->setSprite(oldSprite + "Hit");
		switch (actor->getDirection())
		{
		case DIRECTION::L:
			actor->setAnimation("hit4dL");
			break;
		case DIRECTION::R:
			actor->setAnimation("hit4dR");
			break;
		case DIRECTION::U:
			actor->setAnimation("hit4dB");
			break;
		case DIRECTION::D:
			actor->setAnimation("hit4dF");
			break;
		}
	}
}
