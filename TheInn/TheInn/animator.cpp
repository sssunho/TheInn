#include "animator.h"
#include <sstream>

AnimationManager::AnimationManager()
{
	fstream animationList;

	animationList.open("sprite/animation/list.txt");
	if (!animationList.is_open())
		return;
	
	while (!animationList.eof())
	{
		string animationName;
		string spriteName;
		fstream animationData;
		Animation::AnimationData animation;
		
		animationList >> animationName;
		animationData.open("sprite/animation/" + animationName + ".txt");
		if (!animationData.is_open())
			continue;
		getline(animationData, animation.first);
		while (!animationData.eof())
		{
			Animation::FrameData frame;
			string frameDataBuffer;
			stringstream frameData;
			getline(animationData, frameDataBuffer);
			frameData << frameDataBuffer;

			Animation::SpriteData tempSpriteData;

			while (!frameData.eof())
			{
				frameData >> tempSpriteData.name >> tempSpriteData.x >> tempSpriteData.y;
				frame.push_back(tempSpriteData);
			}

			animation.second.push_back(frame);

		}

		animationDataMap.insert(pair<string, Animation::AnimationData>(animationName, animation));
		animationData.close();

	}

	animationList.close();

}

Animation* AnimationManager::getAnimation(string name)
{
	if (animationDataMap.find(name) == animationDataMap.end())
		return nullptr;
	Animation* instance = new Animation;
	animationInstArr.push_back(instance);
	instance->animationName = name;
	instance->maxFrame = animationDataMap[name].second.size();
	return instance;
}

void Animation::draw(HDC& hdc, int x, int y)
{
	AnimationManager& am = AnimationManager::getInstance();
	SpriteManager& sm = SpriteManager::getInstance();
	FrameData frame = am.animationDataMap[animationName].second[curFrame];

	for (int i = 0; i < frame.size(); i++)
	{
		Sprite sprite = sm.getSprite(am.animationDataMap[animationName].first, frame[i].name);
		sprite.draw(hdc, x + frame[i].x, y + frame[i].y);
	}
}

void AnimationManager::update()
{
	list<Animation*>::iterator it;

	for (it = animationInstArr.begin(); it != animationInstArr.end(); it++)
		(*it)->update();

	for (it = animationInstArr.begin(); it != animationInstArr.end(); )
	{
		if (!(*it)->playing)
			it = animationInstArr.erase(it);
		else
			it++;
	}
}

void Animation::update()
{
	curFrame++;
	if (curFrame >= maxFrame)
	{
		curFrame = 0;
		if(!repeat)
			playing = false;
	}
}