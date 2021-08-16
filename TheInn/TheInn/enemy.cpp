#include "framework.h"
#include "headers.h"

extern RECT clientRect;
int Enemy::temp = 0;

extern HDC hMainDC;

Enemy::Enemy(POINT p, POINT v, const string spriteName, DIRECTION dir, int size, int hp)
	: Actor(p, v, spriteName, dir, size, hp) 
{
	mState = new EnemyIDLE;
	dest = { 1000, 1000 };
	mState->enter(this);
	ObjectManager::registerObj(this);
	detectRange = new Sector(VECTOR(p.x, p.y), 250.0f, 0.0f, PI * 2 / 3);
	atkRange = new Circle(VECTOR(p.x, p.y), 100.0f);
	chaseRange = new Circle(VECTOR(p.x, p.y), 500.0f);
	spriteOffset = { 0, -16 };
}

void Enemy::update(float dt)
{
	Actor::update(dt);

	if (HP <= 0)
		setState(new EnemyDead);

	if(detectRange)
		detectRange->c = pos;
	if(atkRange)
		atkRange->c = pos;
	if(chaseRange)
		chaseRange->c = pos;
	
}

void Enemy::setDirection(DIRECTION d)
{
	Actor::setDirection(d);
	switch (d)
	{
	case DIRECTION::L:
		if (Sector* range = dynamic_cast<Sector*>(detectRange))
			range->direction = PI;
		if (Sector* range = dynamic_cast<Sector*>(atkRange))
			range->direction = PI;
		if (Sector* range = dynamic_cast<Sector*>(chaseRange))
			range->direction = PI;
		break;
	case DIRECTION::R:
		if (Sector* range = dynamic_cast<Sector*>(detectRange))
			range->direction = 0;
		if (Sector* range = dynamic_cast<Sector*>(atkRange))
			range->direction = 0;
		if (Sector* range = dynamic_cast<Sector*>(chaseRange))
			range->direction = 0;
		break;
	case DIRECTION::U:
		if (Sector* range = dynamic_cast<Sector*>(detectRange))
			range->direction = PI / 2;
		if (Sector* range = dynamic_cast<Sector*>(atkRange))
			range->direction = PI / 2;
		if (Sector* range = dynamic_cast<Sector*>(chaseRange))
			range->direction = PI / 2;
		break;
	case DIRECTION::D:
		if (Sector* range = dynamic_cast<Sector*>(detectRange))
			range->direction = -PI / 2;
		if (Sector* range = dynamic_cast<Sector*>(atkRange))
			range->direction = -PI / 2;
		if (Sector* range = dynamic_cast<Sector*>(chaseRange))
			range->direction = -PI / 2;
		break;
	}
}

void Enemy::findPath(VECTOR dest)
{
	while (!path.empty())
		path.pop();
	collider.unset();
	Node* res = ::findPath(pixelToCell(pos), pixelToCell(dest), collider.getSize() + 1);
	collider.set();
	getPath(path, pos, res);
}

void Enemy::draw(HDC& hdc)
{
	POINT drawPoint = pixelToScreen(pos) + spriteOffset;
	animation.draw(hdc, drawPoint.x, drawPoint.y);
	drawHP(hdc, pos, MaxHP, HP);
}

//----------------------------------------------------------------------
//								IDLE
//----------------------------------------------------------------------


StateMachine * EnemyIDLE::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<HIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyOnMove*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyDead*>(state))
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

	Enemy* enemy = dynamic_cast<Enemy*>(obj);
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));

	if (enemy->HP < 0)
	{
		enemy->setState(new EnemyDead);
		return;
	}

	if (player)
	{
		if (enemy->isInDetectArea(player->pos))
		{
			enemy->setState(new EnemyChase);
		}
		else
		{
			if (patrolTimer.getElapsedTime() > 500)
			{
				switch (rand() % 4)
				{
				case 0:
					enemy->setDirection(DIRECTION::L);
					break;
				case 1:
					enemy->setDirection(DIRECTION::R);
					break;
				case 2:
					enemy->setDirection(DIRECTION::U);
					break;
				case 3:
					enemy->setDirection(DIRECTION::D);
					break;
				}
				enemy->setState(new EnemyPatrol);
			}
		}
	}

}

void EnemyIDLE::enter(GameObject * obj)
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

StateMachine * HIT::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<EnemyIDLE*>(state))
	{
		exit(obj);
		dynamic_cast<Enemy*>(obj)->setSprite(oldSprite);
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyChase*>(state))
	{
		exit(obj);
		dynamic_cast<Enemy*>(obj)->setSprite(oldSprite);
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyDead*>(state))
	{
		state->enter(obj);
		return state;
	}

	delete state;
	return nullptr;
}

void HIT::update(GameObject * obj, float dt)
{
	obj->update(dt);
	Enemy* enemy = dynamic_cast<Enemy*>(obj);
	if (enemy->HP < 0)
	{
		enemy->setState(new EnemyDead);
		return;
	}
	if (timer.getElapsedTime() > 300)
	{
		if (dynamic_cast<Enemy*>(obj))
			obj->setState(new EnemyChase);
		else
			obj->setState(new EnemyIDLE);
	}
}

void HIT::enter(GameObject * obj)
{
	timer.set();
	if (Enemy* actor = dynamic_cast<Enemy*>(obj))
	{
		actor->vel = { 0, 0 };
		actor->HP -= damage;
		oldSprite = actor->getSpriteName();
		actor->setSprite(oldSprite + "Hit");

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

void HIT::exit(GameObject* obj)
{
	if (Enemy* enemy = dynamic_cast<Enemy*>(obj))
	{
		Player* player = dynamic_cast<Player*> (ObjectManager::findActor("player"));
		if (player)
		{
			enemy->setDirection(getDirectionFromVector(player->pos - enemy->pos));
		}
	}
}

//----------------------------------------------------------------------
//								OnMove
//----------------------------------------------------------------------

StateMachine * EnemyOnMove::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<EnemyIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyOnMove*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<HIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyDead*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void EnemyOnMove::update(GameObject * obj, float dt)
{
	obj->update(dt);
	Enemy* enemy = dynamic_cast<Enemy*>(obj);
	if (enemy->HP < 0)
	{
		enemy->setState(new EnemyDead);
		return;
	}
}

void EnemyOnMove::enter(GameObject * obj)
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

StateMachine * EnemyPatrol::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<EnemyIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyOnMove*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<HIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyDead*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void EnemyPatrol::update(GameObject * obj, float dt)
{
	EnemyOnMove::update(obj, dt);

	Enemy* enemy = dynamic_cast<Enemy*>(obj);
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	if (enemy->HP < 0)
	{
		enemy->setState(new EnemyDead);
		return;
	}
	if (player)
	{
		if (enemy->isInDetectArea(player->pos))
		{
			enemy->isInDetectArea(player->pos);
			enemy->setState(new EnemyChase);
		}
		else
		{
			if (patrolTimer.getElapsedTime() > 500)
			{
				switch (rand() % 4)
				{
				case 0:
					enemy->setDirection(DIRECTION::L);
					break;
				case 1:
					enemy->setDirection(DIRECTION::R);
					break;
				case 2:
					enemy->setDirection(DIRECTION::U);
					break;
				case 3:
					enemy->setDirection(DIRECTION::D);
					break;
				}
				enemy->setState(new EnemyIDLE);
			}
		}
	}

}

//----------------------------------------------------------------------
//								Chase
//----------------------------------------------------------------------

StateMachine * EnemyChase::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<EnemyIDLE*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyAtk*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<HIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyDead*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

void EnemyChase::update(GameObject * obj, float dt)
{
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	Enemy* enemy = dynamic_cast<Enemy*>(obj);
	if (enemy->HP < 0)
	{
		enemy->setState(new EnemyDead);
		return;
	}

	bool callNewPath = false;

	{
		enemy->animation.update();
		VECTOR ds = dt * enemy->vel;
		VECTOR sign = { ds.x >= 0 ? 1.0f : -1.0f, ds.y >= 0 ? 1.0f : -1.0f };
		enemy->collider.unset();

		while (ds.x * sign.x > 0 || ds.y * sign.y)
		{
			VECTOR unit;
			if (ds.x * sign.x <= 0)
				unit.x = 0;
			else
				unit.x = abs(ds.x) > CELL_PIXEL ? (sign.x) * CELL_PIXEL : ds.x;

			if (ds.y * sign.y <= 0)
				unit.y = 0;
			else
				unit.y = abs(ds.y) > CELL_PIXEL ? (sign.y) * CELL_PIXEL : ds.y;

			enemy->collider.pos = enemy->collider.pos + ds;
			ds = ds - unit;

			if (enemy->collider.collision())
			{
				callNewPath = true;
				break;
			}
			enemy->pos = enemy->collider.pos;

		}
		enemy->collider.update(dt);
		if (enemy->detectRange)
			enemy->detectRange->c = enemy->pos;
		if (enemy->atkRange)
			enemy->atkRange->c = enemy->pos;
		if (enemy->chaseRange)
			enemy->chaseRange->c = enemy->pos;

	}

	if (!enemy->chaseRange->isIn(player->pos))
	{
		obj->setState(new EnemyIDLE);
		return;
	}

	if (enemy->atkRange->isIn(player->pos))
	{
		obj->setState(new EnemyAtk);
		return;
	}

	if (pathUpdateTimer.getElapsedTime() > 1000 || enemy->path.empty() || callNewPath)
	{
		enemy->findPath(player->pos);
		enemy->dest = enemy->path.top();
		enemy->path.pop();
		pathUpdateTimer.set();
		callNewPath = false;
	}
	
	POINT cellPos = pixelToCell(enemy->pos);
	if (cellPos == enemy->dest)
	{
		if (!enemy->path.empty())
		{
			enemy->dest = enemy->path.top();
			enemy->path.pop();
		}
	}

	POINT rel = enemy->dest - POINT(pixelToCell(enemy->pos));
	if (rel.x != 0 && rel.y != 0)
	{
		enemy->vel = (100 / 1.4) * rel;
	}
	else
	{
		enemy->vel = (100) * rel;
		enemy->setDirection(getDirectionFromVector(enemy->vel));
	}

	DIRECTION newDirection = getDirectionFromVector(player->pos - enemy->pos);
	if (newDirection != enemy->dir)
	{
		enemy->dir = newDirection;
		switch (enemy->dir)
		{
		case DIRECTION::L:
			enemy->setAnimation("walk4dL");
			break;
		case DIRECTION::R:
			enemy->setAnimation("walk4dR");
			break;
		case DIRECTION::U:
			enemy->setAnimation("walk4dB");
			break;
		case DIRECTION::D:
			enemy->setAnimation("walk4dF");
			break;
		}
	}

}

void EnemyChase::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	Enemy* enemy = dynamic_cast<Enemy*>(obj);

	enemy->findPath(player->pos);
	enemy->dest = enemy->path.top();
	enemy->path.pop();

	switch (enemy->dir)
	{
	case DIRECTION::L:
		enemy->setAnimation("walk4dL");
		break;
	case DIRECTION::R:
		enemy->setAnimation("walk4dR");
		break;
	case DIRECTION::U:
		enemy->setAnimation("walk4dB");
		break;
	case DIRECTION::D:
		enemy->setAnimation("walk4dF");
		break;
	}

}

//----------------------------------------------------------------------
//								KnockBack
//----------------------------------------------------------------------

StateMachine * EnemyKnockBack::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<EnemyDead*>(state))
	{
		state->enter(obj);
		return state;
	}
	return nullptr;
}

void EnemyKnockBack::update(GameObject * obj, float dt)
{
	Enemy* enemy = dynamic_cast<Enemy*>(obj);
	if (enemy->HP < 0)
	{
		enemy->setState(new EnemyDead);
		return;
	}
}

void EnemyKnockBack::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	Enemy* enemy = dynamic_cast<Enemy*>(obj);

	enemy->setDirection(getDirectionFromVector(player->pos - enemy->pos));

	switch (enemy->getDirection())
	{
	case DIRECTION::L:
		enemy->setAnimation("walk4dL");
		break;
	case DIRECTION::R:
		enemy->setAnimation("walk4dR");
		break;
	case DIRECTION::U:
		enemy->setAnimation("walk4dB");
		break;
	case DIRECTION::D:
		enemy->setAnimation("walk4dF");
		break;
	}

}

StateMachine * EnemyAtk::setState(GameObject * obj, StateMachine * state)
{
	if (dynamic_cast<EnemyChase*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<HIT*>(state))
	{
		state->enter(obj);
		return state;
	}
	if (dynamic_cast<EnemyDead*>(state))
	{
		state->enter(obj);
		return state;
	}
	delete state;
	return nullptr;
}

//----------------------------------------------------------------------
//								Atk
//----------------------------------------------------------------------

void EnemyAtk::update(GameObject * obj, float dt)
{
	obj->update(dt);
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	Enemy* enemy = dynamic_cast<Enemy*>(obj);

	if (enemy->HP < 0)
	{
		enemy->setState(new EnemyDead);
		return;
	}

	if (timer.getElapsedTime() > 1000)
	{
		VECTOR rel = player->pos - enemy->pos;
		enemy->vel = 500 * rel.getUnit();
	}

	if (timer.getElapsedTime() > 1150)
	{
		obj->vel = { 0,0 };
	}
	if(timer.getElapsedTime() > 2000)
		obj->setState(new EnemyChase);

	if ((player->pos - enemy->pos).getScalar() < 32 && !hitten && timer.getElapsedTime() > 1000)
	{
		player->setState(new PlayerHIT);
		hitten = true;
	}
}

void EnemyAtk::enter(GameObject * obj)
{
	Player* player = dynamic_cast<Player*>(ObjectManager::findActor("player"));
	Enemy* enemy = dynamic_cast<Enemy*>(obj);

	if (player)
	{
		VECTOR rel = player->pos - enemy->pos;
		enemy->setDirection(getDirectionFromVector(rel));
		enemy->vel = { 0,0 };
	}
}

//----------------------------------------------------------------------
//								Dead
//----------------------------------------------------------------------

StateMachine * EnemyDead::setState(GameObject * obj, StateMachine * state)
{
	return nullptr;
}


void EnemyDead::update(GameObject * obj, float dt)
{
	obj->destroyObj();
}

void EnemyDead::enter(GameObject * obj)
{
	ObjectManager::createFX("dust", "dust", obj->pos);
}
