#pragma once

#ifndef __ANIMATOR__
#define __ANIMATOR__

#include "sprite.h"
#include "framework.h"
#include <map>
#include <fstream>
#include <vector>
#include <string>

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
	vector<SpriteData> animationQue;
	int curFrame;
	int frameCount;
	bool playing;
	bool repeat;
	
	Animation& operator=(const Animation& ref)
	{
		animationQue = ref.animationQue;
		curFrame = 0;
		frameCount = 0;
		playing = false;
		repeat = false;
	}

	Animation() : animationQue(), curFrame(0), frameCount(0), playing(false), repeat(false) {}

	void update();

public:
	Animation(const Animation& ref)
	{
		animationQue = ref.animationQue;
		curFrame = 0;
		frameCount = 0;
		playing = false;
		repeat = false;
	}

	void play();
	void pause();
	void draw(HDC& hdc, int x, int y);
};

class AnimationManager
{
private:

	AnimationManager();
	AnimationManager(const AnimationManager& ref) {}
	AnimationManager& operator=(const AnimationManager& ref) {}
	~AnimationManager() {};

	map<string, vector<Animation::SpriteData>> animationMap;
	

public:
	static AnimationManager& getInstance()
	{
		static AnimationManager s;
		return s;
	}

	Animation getAnimation(string name);


};

#endif // !__ANIMATOR__

