#include "BT.h"

PlayerBT::PlayerBT(Actor* owner)
{
	mOwner = owner;
	AddChild(new PlayerControl(owner));
}

bool PlayerAtk::Invoke()
{
	TIMER timer;
	if (!GetAsyncKeyState(VK_LBUTTON) & 0x8001 && timer.getElapsedTime() >= 100)
		return false;
	return true;
}
