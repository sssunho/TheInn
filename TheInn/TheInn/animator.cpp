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

			animation.push_back(frame);

		}

		animationDataMap.insert(pair<string, Animation::AnimationData>(animationName, animation));
		animationData.close();

	}

	animationList.close();

}

Animation AnimationManager::getAnimation(string spName, string aniName)
{
	SpriteManager& sm = SpriteManager::getInstance();
	if (animationDataMap.find(aniName) == animationDataMap.end())
		return Animation();
	if (!sm.hasThisImage(spName))
		return Animation();
	Animation instance(spName, aniName);
	instance.maxFrame = animationDataMap[aniName].size();
	return instance;
}

void Animation::draw(HDC& hdc, int x, int y, TransformType flag)
{
	AnimationManager& am = AnimationManager::getInstance();
	SpriteManager& sm = SpriteManager::getInstance();
	FrameData frame = am.animationDataMap[animationName][curFrame];

	for (int i = 0; i < frame.size(); i++)
	{
		Sprite sprite = sm.getSprite(spriteName, frame[i].name);
		sprite.draw(hdc, x + frame[i].x, y + frame[i].y, flag);
	}
}

void AnimationManager::update()
{
	list<Animation*>::iterator it;

	for (it = animationInstArr.begin(); it != animationInstArr.end(); it++)
		(*it)->update();

}

void Animation::update()
{
	curFrame++;
	if (curFrame >= maxFrame)
	{
		curFrame = 0;/*
		if(!repeat)
			playing = false;*/
	}
}