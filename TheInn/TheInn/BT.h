#pragma once
#include "framework.h"
#include "headers.h"

class BTNode
{
protected:
	Actor* mOwner;

public:
	BTNode(Actor* owner = nullptr) : mOwner(owner) {}
	virtual bool Invoke() = 0;
};
class CompositeNode : public BTNode
{
public:
	void AddChild(BTNode *node)
	{
		mChildren.emplace_back(node);
	}
	const std::list<BTNode*>& GetChildren()
	{
		return mChildren;
	}
private:
	std::list<BTNode*> mChildren;
};

class Selector : public CompositeNode
{
public:
	virtual bool Invoke() override
	{
		for (auto node : GetChildren())
		{
			if (node->Invoke())
				return true;
		}
		return false;
	}
};

class Sequence : public CompositeNode
{
public:
	virtual bool Invoke() override
	{
		for (auto node : GetChildren())
		{
			if (!node->Invoke())
				return false;
		}
		return true;
	}
};

//-------------------------------------------------------
//						General
//-------------------------------------------------------


//-------------------------------------------------------
//						Player
//-------------------------------------------------------

class PlayerBT : public Selector
{
private:
	PlayerBT() {};
public:
	PlayerBT(Actor* owner);
};

class PlayerControl : public BTNode
{
public:
	PlayerControl(Actor* owner) : BTNode(owner) {}

	virtual bool Invoke()
	{
		DIRECTION key = getDirectionKeyState();
		ActorState state = mOwner->getState();

		if (key == DIRECTION::NONE)
			mOwner->setState(ActorState::IDLE);
		else
			mOwner->setState(ActorState::ONMOVE);

		if (key != mOwner->getDirection() && key != DIRECTION::NONE)
			mOwner->setDirection(key);

		float speed = 280.0f;
		float dirspeed = 200.0f;

		switch (key)
		{
		case DIRECTION::U:
			mOwner->vel.x = 0;
			mOwner->vel.y = -speed;
			break;
		case DIRECTION::LU:
			mOwner->vel.y = -dirspeed;
			mOwner->vel.x = -dirspeed;
			break;
		case DIRECTION::RU:
			mOwner->vel.y = -dirspeed;
			mOwner->vel.x = dirspeed;
			break;
		case DIRECTION::D:
			mOwner->vel.x = 0;
			mOwner->vel.y = speed;
			break;
		case DIRECTION::LD:
			mOwner->vel.y = dirspeed;
			mOwner->vel.x = -dirspeed;
			break;
		case DIRECTION::RD:
			mOwner->vel.y = dirspeed;
			mOwner->vel.x = dirspeed;
			break;
		case DIRECTION::L:
			mOwner->vel.y = 0;
			mOwner->vel.x = -speed;
			break;
		case DIRECTION::R:
			mOwner->vel.y = 0;
			mOwner->vel.x = speed;
			break;
		default:
			mOwner->vel.x = 0;
			mOwner->vel.y = 0;
			break;
		}
		return true;
	}
};

class PlayerAtk : public BTNode
{
public:
	virtual bool Invoke();
};