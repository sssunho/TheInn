#include "fsm.h"

void StateMachine::update(GameObject * obj, float dt)
{
	obj->update(dt);
}

//----------------------------------------
//			PlayerIDLE
//----------------------------------------

StateMachine* PlayerIDLE::setState(GameObject * obj, StateMachine* state)
{
	if (dynamic_cast<PlayerMove*>(state))
	{
		state->enter(obj);
		return state;
	}
	else if(dynamic_cast<Attack*>(state))
	{
		state->enter(obj);
		return state;
	}

	delete state;
	return nullptr;
}

void PlayerIDLE::update(GameObject* obj, float dt)
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
		obj->setState(new PlayerCombo1);
	else if (getDirectionKeyState() != DIRECTION::NONE)
		obj->setState(new PlayerMove);
	else
	{
		obj->vel = { 0, 0 };
	}
}

void PlayerIDLE::enter(GameObject* obj)
{
	Player* player = dynamic_cast<Player*>(obj);
	obj->vel = { 0,0 };
	switch (player->getDirection())
	{
	case DIRECTION::D:
		player->setAnimation("standF");
		break;
	case DIRECTION::R:
	case DIRECTION::L:
		player->setAnimation("standS");
		break;
	case DIRECTION::U:
		player->setAnimation("standB");
		break;
	case DIRECTION::RU:
	case DIRECTION::LU:
		player->setAnimation("standBD");
		break;
	case DIRECTION::LD:
	case DIRECTION::RD:
		player->setAnimation("standFD");
		break;
	}
}

//----------------------------------------
//			PlayerMOVE
//----------------------------------------

StateMachine* PlayerMove::setState(GameObject* obj, StateMachine* state)
{
	if (dynamic_cast<PlayerIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<PlayerMove*>(state))
	{
		state->enter(obj);
		return state;
	}
	else if (dynamic_cast<Attack*>(state))
	{
		state->enter(obj);
		return state;
	}

	delete state;
	return nullptr;
}

void PlayerMove::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(obj);

	switch (player->getDirection())
	{
	case DIRECTION::D:
		player->setAnimation("walkF");
		break;
	case DIRECTION::R:
	case DIRECTION::L:
		player->setAnimation("walkS");
		break;
	case DIRECTION::U:
		player->setAnimation("walkB");
		break;
	}
}

void PlayerMove::update(GameObject* obj, float dt)
{
	StateMachine::update(obj, dt);
	Player* player = dynamic_cast<Player*>(obj);
	DIRECTION key = getDirectionKeyState();
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		obj->setState(new PlayerCombo1);
	}
	else if (key == DIRECTION::NONE)
		obj->setState(new PlayerIDLE);
	else if(key != player->getDirection())
	{
		player->setDirection(key);
		player->setState(new PlayerMove);
	}
	else
	{
		float speed = 200;
		float dirSpeed = speed / 1.4;
		switch (player->getDirection())
		{
		case DIRECTION::D:
			player->vel = { 0, speed };
			break;
		case DIRECTION::R:
			player->vel = { speed, 0 };
			break;
		case DIRECTION::L:
			player->vel = { -speed, 0 };
			break;
		case DIRECTION::U:
			player->vel = { 0, -speed };
			break;
		case DIRECTION::RU:
			player->vel = { dirSpeed, -dirSpeed };
			break;
		case DIRECTION::LU:
			player->vel = { -dirSpeed, -dirSpeed };
			break;
		case DIRECTION::LD:
			player->vel = { -dirSpeed, dirSpeed };
			break;
		case DIRECTION::RD:
			player->vel = { dirSpeed, dirSpeed };
			break;
		}
	}
}

//----------------------------------------
//			PlayerAtk
//----------------------------------------

StateMachine * PlayerCombo1::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<PlayerIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<PlayerCombo2*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void PlayerCombo1::update(GameObject * obj, float dt)
{
	Player* player = dynamic_cast<Player*>(obj);
	StateMachine::update(obj, dt);

	if (timer.getElapsedTime() > 300 && pretime)
	{
		player->setSpeed(500.0);
		pretime = false;
	}
	if (timer.getElapsedTime() > 350 && hittime)
	{
		hittime = false;
		player->setSpeed(0);
		switch (player->getDirection())
		{
		case DIRECTION::L:
			ObjectManager::createFX("slash", "slashfxS", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 48, -PI, PI), 10, damageTo);
			break;
		case DIRECTION::R:
			ObjectManager::createFX("slash", "slashfxS", player->pos, TF_YFLIP);
			ObjectManager::forEachInArea(&Sector(obj->pos, 48, 0, PI), 10, damageTo);
			break;
		case DIRECTION::U:
			ObjectManager::createFX("slash", "slashfxB", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 48, -PI/2, PI), 10, damageTo);
			break;
		case DIRECTION::D:
			ObjectManager::createFX("slash", "slashfxF", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 48, PI/2, PI), 10, damageTo);
			break;
		case DIRECTION::LU:
			ObjectManager::createFX("slash", "slashfxBD", player->pos, 0);
			break;
		case DIRECTION::RU:
			ObjectManager::createFX("slash", "slashfxBD", player->pos, TF_YFLIP);
			break;
		case DIRECTION::LD:
			ObjectManager::createFX("slash", "slashfxFD", player->pos, 0);
			break;
		case DIRECTION::RD:
			ObjectManager::createFX("slash", "slashfxFD", player->pos, TF_YFLIP);
			break;
		}
	}

	if (!player->animationPlaying())
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
			player->setState(new PlayerCombo2);
		else
			player->setState(new PlayerIDLE);
	}
}

void PlayerCombo1::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(obj);
	player->setSpeed(0);
	timer.set();
	switch (player->getDirection())
	{
	case DIRECTION::D:
		player->setAnimation("vslashF", false);
		break;
	case DIRECTION::R:
	case DIRECTION::L:
		player->setAnimation("vslashS", false);
		break;
	case DIRECTION::U:
		player->setAnimation("vslashB", false);
		break;
	case DIRECTION::RU:
	case DIRECTION::LU:
		player->setAnimation("vslashBD", false);
		break;
	case DIRECTION::LD:
	case DIRECTION::RD:
		player->setAnimation("vslashFD", false);
		break;
	}
}

StateMachine * PlayerCombo2::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<PlayerIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<PlayerCombo3*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void PlayerCombo2::update(GameObject * obj, float dt)
{
	Player* player = dynamic_cast<Player*>(obj);
	StateMachine::update(obj, dt);

	if (timer.getElapsedTime() > 300 && pretime)
	{
		player->setSpeed(500.0);
		pretime = false;
	}
	if (timer.getElapsedTime() > 350 && hittime)
	{
		hittime = false;
		player->setSpeed(0);
		switch (player->getDirection())
		{
		case DIRECTION::L:
			ObjectManager::createFX("slash", "rslashfxS", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 48, PI, PI * 2 / 3), 10, damageTo);
			break;
		case DIRECTION::R:
			ObjectManager::createFX("slash", "rslashfxS", player->pos, TF_YFLIP);
			ObjectManager::forEachInArea(&Sector(obj->pos, 48, 0, PI * 2 / 3), 10, damageTo);
			break;
		case DIRECTION::U:
			ObjectManager::createFX("slash", "rslashfxB", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 48, -PI / 2, PI * 2 / 3), 10, damageTo);
			break;
		case DIRECTION::D:
			ObjectManager::createFX("slash", "rslashfxF", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 48, PI / 2, PI * 2 / 3), 10, damageTo);
			break;
		case DIRECTION::LU:
			ObjectManager::createFX("slash", "rslashfxBD", player->pos, 0);
			break;
		case DIRECTION::RU:
			ObjectManager::createFX("slash", "rslashfxBD", player->pos, TF_YFLIP);
			break;
		case DIRECTION::LD:
			ObjectManager::createFX("slash", "rslashfxFD", player->pos, 0);
			break;
		case DIRECTION::RD:
			ObjectManager::createFX("slash", "rslashfxFD", player->pos, TF_YFLIP);
			break;
		}
	}

	if (!player->animationPlaying())
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
			player->setState(new PlayerCombo3);
		else
			player->setState(new PlayerIDLE);
	}
}

void PlayerCombo2::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(obj);
	switch (player->getDirection())
	{
	case DIRECTION::D:
		player->setAnimation("hslashF", false);
		break;
	case DIRECTION::R:
	case DIRECTION::L:
		player->setAnimation("hslashS", false);
		break;
	case DIRECTION::U:
		player->setAnimation("hslashB", false);
		break;
	case DIRECTION::RU:
	case DIRECTION::LU:
		player->setAnimation("hslashBD", false);
		break;
	case DIRECTION::LD:
	case DIRECTION::RD:
		player->setAnimation("hslashFD", false);
		break;
	}
}

StateMachine * PlayerCombo3::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<PlayerIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void PlayerCombo3::update(GameObject * obj, float dt)
{
	Player* player = dynamic_cast<Player*>(obj);
	StateMachine::update(obj, dt);

	if (timer.getElapsedTime() > 300 && pretime)
	{
		player->setSpeed(500.0);
		pretime = false;
	}
	if (timer.getElapsedTime() > 350 && hittime)
	{
		hittime = false;
		player->setSpeed(0);
	}

	if (!player->animationPlaying() && timer.getElapsedTime() > 800)
	{
		player->setState(new PlayerIDLE);
	}
}

void PlayerCombo3::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(obj);
	switch (player->getDirection())
	{
	case DIRECTION::D:
		player->setAnimation("stabF", false);
		break;
	case DIRECTION::R:
	case DIRECTION::L:
		player->setAnimation("stabS", false);
		break;
	case DIRECTION::U:
		player->setAnimation("stabB", false);
		break;
	case DIRECTION::RU:
	case DIRECTION::LU:
		player->setAnimation("stabBD", false);
		break;
	case DIRECTION::LD:
	case DIRECTION::RD:
		player->setAnimation("stabFD", false);
		break;
	}
}

//----------------------------------------
//			PlayerHit
//----------------------------------------

StateMachine * PlayerHIT::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<PlayerHIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<PlayerIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void PlayerHIT::update(GameObject * obj, float dt)
{
	Player* player = dynamic_cast<Player*>(obj);
	if (timer.getElapsedTime() > 300)
		player->setState(new PlayerIDLE);
}

void PlayerHIT::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(obj);
	player->setAnimation("hit", true);
}