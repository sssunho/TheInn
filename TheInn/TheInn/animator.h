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
	friend AnimationManager;

private:
	struct SpriteData
	{
		string name;
		int x;
		int y;
	};

	typedef vector<SpriteData> FrameData;
	typedef pair<string,vector<FrameData>> AnimationData;

	string animationName;
	int curFrame;
	int maxFrame;
	bool playing;
	bool repeat;
		
	Animation& operator=(const Animation& ref)
	{
		animationName = ref.animationName;
		curFrame = 0;
		maxFrame = 0;
		playing = true;
		repeat = false;
	}

	Animation() : animationName(), curFrame(0), maxFrame(0), playing(true), repeat(false) {}

	void update();

public:
	Animation(const Animation& ref)
	{
		animationName = ref.animationName;
		curFrame = 0;
		maxFrame = 0;
		playing = true;
		repeat = false;
	}

	void release() { playing = false; }

	void draw(HDC& hdc, int x, int y);
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

public:
	static AnimationManager& getInstance()
	{
		static AnimationManager s;
		return s;
	}

	Animation* getAnimation(string name);

	void update();

};

#endif // !__ANIMATOR__

