#include "magician.h"

Magician::Magician(VECTOR p) : Enemy(p, { 0,0 }, "magician", DIRECTION::D, 2, 40)
{
	chargeAni = AnimationManager::getInstance().getAnimation("darkness", "darkness-charge", false);
	mState = new MagicianIDLE;
	mState->enter(this);
	setDetectRange(new Sector(pos, 300.0f, PI / 2, PI * 2 / 3));
	setAtkRange(new Circle(pos, 300.0f));
	setChaseRange(new Circle(pos, 300.0f));
	
}

Magician::Magician(VECTOR p, DIRECTION d, float range)
	: Enemy(p, { 0,0 }, "magician", d, 2, 40)
{
	chargeAni = AnimationManager::getInstance().getAnimation("darkness", "darkness-charge", false);
	mState = new MagicianIDLE;
	mState->enter(this);
	setAtkRange(new Circle(pos, 300.0f));
	setChaseRange(new Circle(pos, 300.0f));
	switch (d)
	{
	case DIRECTION::L:
		setDetectRange(new Sector(pos, 300.0f, PI, PI * 2 / 3));
		break;
	case DIRECTION::R:
		setDetectRange(new Sector(pos, 300.0f, 0, PI * 2 / 3));
		break;
	case DIRECTION::U:
		setDetectRange(new Sector(pos, 300.0f, -PI / 2, PI * 2 / 3));
		break;
	case DIRECTION::D:
		setDetectRange(new Sector(pos, 300.0f, PI / 2, PI * 2 / 3));
		break;
	}
}

void Magician::update(float dt)
{
	Enemy::update(dt);
	if (dynamic_cast<MagicianAtk*>(mState))
	{
		chargeAni.update();
	}
	if (HP <= 0)
		setState(new MagicianDead);
}

void Magician::draw(HDC & hdc)
{
	Enemy::draw(hdc);
	if (dynamic_cast<MagicianAtk*>(mState))
	{
		POINT drawPoint = pixelToScreen(pos);
		chargeAni.draw(hdc, drawPoint.x, drawPoint.y - 64);
	}

}

//----------------------------------------------------------------------
//								IDLE
//----------------------------------------------------------------------


StateMachine * MagicianIDLE::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<MagicianHIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianOnMove*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianAtk*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianDead*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void MagicianIDLE::update(GameObject * obj, float dt)
{
	obj->update(dt);

	Magician* magician = dynamic_cast<Magician*>(obj);
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));

	if (magician->HP < 0)
	{
		magician->setState(new MagicianDead);
		return;
	}

	if (player)
	{
		if (magician->isInDetectArea(player->pos))
		{
			magician->setState(new MagicianAtk);
			POINT emotion = magician->pos;
			emotion.y += 1;
			ObjectManager::createFX("emotion", "discover", emotion);
		}
		else
		{
			if (patrolTimer.getElapsedTime() > 500)
			{
				switch (rand() % 4)
				{
				case 0:
					magician->setDirection(DIRECTION::L);
					break;
				case 1:
					magician->setDirection(DIRECTION::R);
					break;
				case 2:
					magician->setDirection(DIRECTION::U);
					break;
				case 3:
					magician->setDirection(DIRECTION::D);
					break;
				}
				magician->setState(new MagicianPatrol);
			}
		}
	}

}

void MagicianIDLE::enter(GameObject * obj)
{
	if (Actor* actor = dynamic_cast<Actor*>(obj))
	{
		actor->setSpeed(0);
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

//----------------------------------------------------------------------
//								HIT
//----------------------------------------------------------------------

StateMachine * MagicianHIT::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<MagicianIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianAtk*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianDead*>(state))
	{
		state->enter(obj);
		return state;
	}

	delete state;
	return nullptr;
}

void MagicianHIT::update(GameObject * obj, float dt)
{
	obj->update(dt);
	Magician* magician = dynamic_cast<Magician*>(obj);
	if (magician->HP < 0)
	{
		magician->setState(new MagicianDead);
		return;
	}
	if (timer.getElapsedTime() > 300)
	{
		if (dynamic_cast<Magician*>(obj))
			obj->setState(new MagicianAtk);
		else
			obj->setState(new MagicianIDLE);
	}
}

void MagicianHIT::enter(GameObject * obj)
{
	timer.set();
	if (Magician* actor = dynamic_cast<Magician*>(obj))
	{
		actor->vel = { 0, 0 };
		actor->HP -= damage;

		obj->vel = { 0, 0 };
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

//----------------------------------------------------------------------
//								OnMove
//----------------------------------------------------------------------

StateMachine * MagicianOnMove::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<MagicianIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianOnMove*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianHIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianDead*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void MagicianOnMove::update(GameObject * obj, float dt)
{
	obj->update(dt);
	Magician* magician = dynamic_cast<Magician*>(obj);
	if (magician->HP < 0)
	{
		magician->setState(new MagicianDead);
		return;
	}
}

void MagicianOnMove::enter(GameObject * obj)
{
	if (Actor* actor = dynamic_cast<Actor*>(obj))
	{
		actor->setSpeed(100);
		switch (actor->getDirection())
		{
		case DIRECTION::L:
			actor->setAnimation("walk4dL");
			break;
		case DIRECTION::R:
			actor->setAnimation("walk4dR");
			break;
		case DIRECTION::U:
			actor->setAnimation("walk4dB");
			break;
		case DIRECTION::D:
			actor->setAnimation("walk4dF");
			break;
		}
	}
}

//----------------------------------------------------------------------
//								Patrol
//----------------------------------------------------------------------

StateMachine * MagicianPatrol::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<MagicianIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianOnMove*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianHIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianDead*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianAtk*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void MagicianPatrol::update(GameObject * obj, float dt)
{
	MagicianOnMove::update(obj, dt);

	Magician* magician = dynamic_cast<Magician*>(obj);
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	if (magician->HP < 0)
	{
		magician->setState(new MagicianDead);
		return;
	}
	if (player)
	{
		if (magician->isInDetectArea(player->pos))
		{
			magician->isInDetectArea(player->pos);
			POINT emotion = magician->pos;
			emotion.y += 1;
			ObjectManager::createFX("emotion", "discover", emotion);
			magician->setState(new MagicianAtk);
		}
		else
		{
			if (patrolTimer.getElapsedTime() > 500)
			{
				switch (rand() % 4)
				{
				case 0:
					magician->setDirection(DIRECTION::L);
					break;
				case 1:
					magician->setDirection(DIRECTION::R);
					break;
				case 2:
					magician->setDirection(DIRECTION::U);
					break;
				case 3:
					magician->setDirection(DIRECTION::D);
					break;
				}
				magician->setState(new MagicianIDLE);
			}
		}
	}

}

//----------------------------------------------------------------------
//								Atk
//----------------------------------------------------------------------

StateMachine * MagicianAtk::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<MagicianHIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianAtk*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<MagicianDead*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void MagicianAtk::update(GameObject * obj, float dt)
{
	obj->update(dt);
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	Magician* magician = dynamic_cast<Magician*>(obj);

	if (magician->HP < 0)
	{
		magician->setState(new MagicianDead);
		return;
	}

	if (timer.getElapsedTime() < 300)
		return;

	VECTOR rel = player->pos - magician->pos;
	magician->setDirection(getDirectionFromVector(rel));

	switch (magician->getDirection())
	{
	case DIRECTION::L:
		magician->setAnimation("stand4dL");
		break;
	case DIRECTION::R:
		magician->setAnimation("stand4dR");
		break;
	case DIRECTION::U:
		magician->setAnimation("stand4dB");
		break;
	case DIRECTION::D:
		magician->setAnimation("stand4dF");
		break;
	}

	if (!magician->isInAtkArea(player->pos))
	{
		ObjectManager::createFX("emotion", "lost", magician->pos);
		magician->setState(new MagicianIDLE);
	}

	if (!magician->chargeAni.isPlaying() && !posttime)
	{
		VECTOR rel = player->pos - magician->pos + VECTOR(0, 64);
		POINT p = { magician->pos.x, magician->pos.y - 64 };
		rel = 300 * rel.getUnit();
		new Projectile(p, rel, "darkness");
		magician->chargeAni = Animation();
		posttime = true;
	}

	if (timer.getElapsedTime() > 2000)
		obj->setState(new MagicianAtk);
}

void MagicianAtk::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	Magician* magician = dynamic_cast<Magician*>(obj);

	if (player)
	{
		VECTOR rel = player->pos - magician->pos;
		magician->setDirection(getDirectionFromVector(rel));
		magician->vel = { 0,0 };
		magician->chargeAni = AnimationManager::getInstance().getAnimation("darkness", "darkness-charge", false);
	}
}

//----------------------------------------------------------------------
//								Dead
//----------------------------------------------------------------------

StateMachine * MagicianDead::setState(GameObject * obj, StateMachine * state)
{
	return nullptr;
}


void MagicianDead::update(GameObject * obj, float dt)
{
	obj->destroyObj();
}

void MagicianDead::enter(GameObject * obj)
{
	ObjectManager::createFX("dust", "dust", obj->pos);
}
