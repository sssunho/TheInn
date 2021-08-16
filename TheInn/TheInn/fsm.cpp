#include "headers.h"

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
	if (dynamic_cast<PlayerHIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<PlayerDodge*>(state))
	{
		state->enter(obj);
		return state;
	}


	delete state;
	return nullptr;
}

void PlayerIDLE::update(GameObject* obj, float dt)
{
	Player* player = dynamic_cast<Player*>(obj);
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8001)
		obj->setState(new PlayerCombo1);
	else if (getDirectionKeyState() != DIRECTION::NONE)
		obj->setState(new PlayerMove);
	else if (GetAsyncKeyState(VK_SPACE) & 0x8000 && player->canDodge())
		obj->setState(new PlayerDodge);
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
	if (dynamic_cast<Attack*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<PlayerHIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<PlayerDodge*>(state))
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
	DIRECTION curDir = player->getDirection();

	static DIRECTION d = DIRECTION::NONE;

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		obj->setState(new PlayerCombo1);
		return;
	}
	else if (key == DIRECTION::NONE)
	{
		obj->setState(new PlayerIDLE);
		return;
	}
	else if (GetAsyncKeyState(VK_SPACE) & 0x8000 && player->canDodge())
	{
		obj->setState(new PlayerDodge);
		return;
	}


	float speed = 200;
	float dirSpeed = 200 / 1.4;
	switch (key)
	{
	case DIRECTION::D:
		player->vel = { 0, speed };
		break;
	case DIRECTION::R:
		player->vel = { speed, 0 };
		break;
	case DIRECTION::RD:
		player->vel = { dirSpeed, dirSpeed };
		break;
	case DIRECTION::U:
		player->vel = { 0, -speed };
		break;
	case DIRECTION::RU:
		player->vel = { dirSpeed, -dirSpeed };
		break;
	case DIRECTION::L:
		player->vel = { -speed, 0 };
		break;
	case DIRECTION::LD:
		player->vel = { -dirSpeed, dirSpeed };
		break;
	case DIRECTION::LU:
		player->vel = { -dirSpeed, -dirSpeed };
		break;
	default:
		break;
	}

	switch (curDir)
	{
	case DIRECTION::L:
		if (!(L_KEY_STATE & 0x8001))
		{
			if (R_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::R);
			if (U_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::U);
			if (D_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::D);
			player->setState(new PlayerMove);
		}
		break;
	case DIRECTION::R:
		if (!(R_KEY_STATE & 0x8001))
		{
			if (L_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::L);
			if (U_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::U);
			if (D_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::D);
			player->setState(new PlayerMove);
		}
		break;
	case DIRECTION::U:
		if (!(U_KEY_STATE & 0x8001))
		{
			if (R_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::R);
			if (L_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::L);
			if (D_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::D);
			player->setState(new PlayerMove);
		}
		break;
	case DIRECTION::D:
		if (!(D_KEY_STATE & 0x8001))
		{
			if (R_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::R);
			if (U_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::U);
			if (L_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::L);
			player->setState(new PlayerMove);
		}
		break;
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
	if (dynamic_cast<PlayerDodge*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<PlayerHIT*>(state))
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

	if (timer.getElapsedTime() < 300)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && player->canDodge())
		{
			obj->setState(new PlayerDodge);
			return;
		}
	}

	if (timer.getElapsedTime() > 300 && pretime)
	{
		if (getDirectionKeyState() == player->getDirection())
			player->setSpeed(400.0);
		pretime = false;
	}
	if (timer.getElapsedTime() > 350 && hittime)
	{
		VECTOR center = obj->pos;
		hittime = false;
		player->setSpeed(0);
		switch (player->getDirection())
		{
		case DIRECTION::L:
			center.x += 8;
			ObjectManager::createFX("slash", "slashfxS", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 80, -PI, PI), 10, slashAtk);
			break;
		case DIRECTION::R:
			center.x -= 8;
			ObjectManager::createFX("slash", "slashfxS", player->pos, TF_YFLIP);
			ObjectManager::forEachInArea(&Sector(obj->pos, 80, 0, PI), 10, slashAtk);
			break;
		case DIRECTION::U:
			center.y += 8;
			ObjectManager::createFX("slash", "slashfxB", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 80, -PI/2, PI), 10, slashAtk);
			break;
		case DIRECTION::D:
			center.y -= 8;
			ObjectManager::createFX("slash", "slashfxF", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 80, PI/2, PI), 10, slashAtk);
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
	player->vel = { 0,0 };
	timer.set();

	switch (player->getDirection())
	{
	case DIRECTION::L:
		if (!(L_KEY_STATE & 0x8001))
		{
			if (R_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::R);
			if (U_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::U);
			if (D_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::D);
		}
		break;
	case DIRECTION::R:
		if (!(R_KEY_STATE & 0x8001))
		{
			if (L_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::L);
			if (U_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::U);
			if (D_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::D);
		}
		break;
	case DIRECTION::U:
		if (!(U_KEY_STATE & 0x8001))
		{
			if (R_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::R);
			if (L_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::L);
			if (D_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::D);
		}
		break;
	case DIRECTION::D:
		if (!(D_KEY_STATE & 0x8001))
		{
			if (R_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::R);
			if (U_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::U);
			if (L_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::L);
		}
		break;
	}



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
	if (dynamic_cast<PlayerHIT*>(state))
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
		if (getDirectionKeyState() == player->getDirection())
			player->setSpeed(400.0);
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
			ObjectManager::forEachInArea(&Sector(obj->pos, 80, PI, PI), 10, slashAtk);
			break;
		case DIRECTION::R:
			ObjectManager::createFX("slash", "rslashfxS", player->pos, TF_YFLIP);
			ObjectManager::forEachInArea(&Sector(obj->pos, 80, 0, PI), 10, slashAtk);
			break;
		case DIRECTION::U:
			ObjectManager::createFX("slash", "rslashfxB", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 80, -PI / 2, PI), 10, slashAtk);
			break;
		case DIRECTION::D:
			ObjectManager::createFX("slash", "rslashfxF", player->pos, 0);
			ObjectManager::forEachInArea(&Sector(obj->pos, 80, PI / 2, PI), 10, slashAtk);
			break;
		}
	}

	if (timer.getElapsedTime() > 700)
	{
		player->setState(new PlayerIDLE);
	}
}

void PlayerCombo2::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(obj);

	switch (player->getDirection())
	{
	case DIRECTION::L:
		if (!(L_KEY_STATE & 0x8001))
		{
			if (R_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::R);
			if (U_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::U);
			if (D_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::D);
		}
		break;
	case DIRECTION::R:
		if (!(R_KEY_STATE & 0x8001))
		{
			if (L_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::L);
			if (U_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::U);
			if (D_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::D);
		}
		break;
	case DIRECTION::U:
		if (!(U_KEY_STATE & 0x8001))
		{
			if (R_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::R);
			if (L_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::L);
			if (D_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::D);
		}
		break;
	case DIRECTION::D:
		if (!(D_KEY_STATE & 0x8001))
		{
			if (R_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::R);
			if (U_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::U);
			if (L_KEY_STATE & 0x8001)
				player->setDirection(DIRECTION::L);
		}
		break;
	}



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
		if(getDirectionKeyState() == player->getDirection())
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
	{
		player->setState(new PlayerIDLE);
	}
}

void PlayerHIT::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(obj);
	player->setAnimation("hit", true);
	ObjectManager::createFX("hitten", "hitten", player->pos + VECTOR(0, -10));
}

StateMachine * PlayerDodge::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<PlayerIDLE*>(state))
	{
		Player* player = dynamic_cast<Player*>(obj);
		player->unsetDodge();
		state->enter(obj);
		
		return state;
	}
	if (dynamic_cast<PlayerHIT*>(state))
	{
		if (timer.getElapsedTime() > 100)
		{
			Player* player = dynamic_cast<Player*>(obj);
			state->enter(obj);
			player->unsetDodge();
			return state;
		}
	}
	delete state;
	return nullptr;
}

void PlayerDodge::update(GameObject * obj, float dt)
{
	Player* player = dynamic_cast<Player*>(obj);
	obj->update(dt);
	if (timer.getElapsedTime() > 66)
	{
		obj->vel = { 0,0 };
	}
	if (timer.getElapsedTime() > 100)
		player->unsetDodge();
	if (timer.getElapsedTime() > 200)
		obj->setState(new PlayerIDLE);
}

void PlayerDodge::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(obj);
	player->setDodgeTimer();
	player->setDodge();
	DIRECTION key = getDirectionKeyState();
	float speed = 500.0f;
	float dirSpeed = speed / 1.414f;
	SoundManager::playSE("Evasion1.ogg");

	switch (key)
	{
	case DIRECTION::L:
		obj->vel = { -speed, 0 };
		return;
	case DIRECTION::R:
		obj->vel = { speed, 0 };
		return;
	case DIRECTION::U:
		obj->vel = { 0, -speed };
		return;
	case DIRECTION::D:
		obj->vel = { 0, speed };
		return;
	case DIRECTION::LD:
		obj->vel = { -dirSpeed, dirSpeed };
		return;
	case DIRECTION::LU:
		obj->vel = { -dirSpeed, -dirSpeed };
		return;
	case DIRECTION::RD:
		obj->vel = { dirSpeed, dirSpeed };
		return;
	case DIRECTION::RU:
		obj->vel = { dirSpeed, -dirSpeed }; 
		return;
	}

	switch (player->getDirection())
	{
	case DIRECTION::L:
		obj->vel = { -speed, 0 };
		break;
	case DIRECTION::R:
		obj->vel = { speed, 0 };
		break;
	case DIRECTION::U:
		obj->vel = { 0, -speed };
		break;
	case DIRECTION::D:
		obj->vel = { 0, speed };
		break;
	}
}
