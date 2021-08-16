#include "projectile.h"

Projectile::Projectile(POINT p, POINT v, const string spriteName, int timeLimit)
	: Actor(p, v, spriteName, DIRECTION::D, 0), timeLimit(timeLimit)
{
	ObjectManager::registerObj(this);
	animation = AnimationManager::getInstance().getAnimation(spriteName, spriteName, true);
}

void Projectile::update(float dt)
{
	Actor::update(dt);
	if (timer.getElapsedTime() > timeLimit)
		destroyObj();
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	if (!player->isDodge())
	{
		if (Circle(pos, 16).isIn(player->pos))
		{
			player->setState(new PlayerHIT);
			destroyObj();
		}
	}
}