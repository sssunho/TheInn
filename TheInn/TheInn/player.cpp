#include "player.h"
#include "headers.h"

Player::Player(POINT p) : Actor(p, { 0, 0 }, "swordman", DIRECTION::D, 2)
{
	mState = new PlayerIDLE;
	mState->enter(this);
	ObjectManager::registerObj("player", this);
}

