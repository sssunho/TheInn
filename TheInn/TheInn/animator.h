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
	friend class AnimationObj;
	friend class Actor;

private:
	struct SpriteData
	{
		string name;
		int x;
		int y;
	};

	typedef pair<vector<SpriteData>, string> FrameData;
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
		repeat = ref.repeat;
		collapse = false;
	}

	Animation& operator=(const Animation& ref)
	{
		animationName = ref.animationName;
		spriteName = ref.spriteName;
		curFrame = 0;
		maxFrame = ref.maxFrame;
		playing = true;
		repeat = ref.repeat;
		collapse = false;
		return *this;
	}

	void update();

	void release() { collapse = true; }

	void draw(HDC& hdc, int x, int y, int flag = 0);

	void setRepeat(bool flag) { repeat = flag; }

	bool isPlaying() { return playing; }

	void rewind() { curFrame = 0; playing = true; }

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
	list<Animation*> spriteFXList;
	void eraseInst(Animation* ani) { spriteFXList.remove(ani); }

public:

	static AnimationManager& getInstance()
	{
		static AnimationManager s;
		return s;
	}

	Animation getAnimation(string spName, string aniName, bool repeat = true);

	void update();

	void draw();

};

#endif // !__ANIMATOR__

