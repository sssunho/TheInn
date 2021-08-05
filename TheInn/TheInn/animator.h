#pragma once

#ifndef __ANIMATOR__
#define __ANIMATOR__

#include "sprite.h"
#include "framework.h"
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <list>

using namespace std;

class AnimationManager;

class Animation
{
	friend class AnimationManager;
	friend class Actor;

private:
	struct SpriteData
	{
		string name;
		int x;
		int y;
	};

	typedef vector<SpriteData> FrameData;
	typedef vector<FrameData> AnimationData;

	string animationName;
	string spriteName;
	int curFrame;
	int maxFrame;
	bool playing;
	bool repeat;
	bool collapse;
		
	Animation() : 
		animationName(), spriteName(),
		curFrame(0), maxFrame(0), 
		playing(true), repeat(false), collapse(false) {}

	Animation(string spName, string aniName) :
		animationName(aniName), spriteName(spName),
		curFrame(0), maxFrame(0),
		playing(true), repeat(false), collapse(false) {}

public:
	Animation(const Animation& ref)
	{
		animationName = ref.animationName;
		spriteName = ref.spriteName;
		curFrame = 0;
		maxFrame = ref.maxFrame;
		playing = true;
		repeat = false;
		collapse = false;
	}

	Animation& operator=(const Animation& ref)
	{
		animationName = ref.animationName;
		spriteName = ref.spriteName;
		curFrame = 0;
		maxFrame = ref.maxFrame;
		playing = true;
		repeat = false;
		collapse = false;
		return *this;
	}

	void update();

	void release() { collapse = true; }

	void draw(HDC& hdc, int x, int y, TransformType flag = NONE);

	void setRepeat(bool flag) { repeat = flag; }
};



class AnimationManager
{
	friend class Animation;

private:

	AnimationManager();
	AnimationManager(const AnimationManager& ref) {}
	AnimationManager& operator=(const AnimationManager& ref) {}
	~AnimationManager() {};

	map<string, Animation::AnimationData> animationDataMap;
	list<Animation*> animationInstArr;
	void eraseInst(Animation* ani) { animationInstArr.remove(ani); }

public:

	static AnimationManager& getInstance()
	{
		static AnimationManager s;
		return s;
	}

	Animation getAnimation(string, string);

	void update();

};

#endif // !__ANIMATOR__

