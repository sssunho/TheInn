#include "player.h"
#include "headers.h"

Player::Player(POINT p) : Actor(p, { 0, 0 }, "swordman", DIRECTION::D, 2)
{
	dodge = false;
	mState = new PlayerIDLE;
	mState->enter(this);
	ObjectManager::registerObj("player", this);
}

void Player::draw(HDC& hdc)
{
	Actor::draw(hdc);
	drawHP(hdc, pos, MaxHP, HP);
}

void slashAtk(GameObject* target, int damage)
{
	Enemy* enemy;
	if (enemy = dynamic_cast<Enemy*>(target))
	{
		enemy->HP -= damage;
		if (dynamic_cast<Magician*>(enemy))
			enemy->setState(new MagicianHIT);
		else
			enemy->setState(new HIT);
	}
}

void stabAtk(GameObject* target, int damage)
{
	Enemy* enemy;
	if (enemy = dynamic_cast<Enemy*>(target))
	{
		enemy->HP -= damage;
		enemy->setState(new HIT);
	}
}