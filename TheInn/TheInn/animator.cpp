#include "headers.h"
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
			char ch;
			Animation::FrameData frame;
			string frameDataBuffer;
			stringstream frameData;
			getline(animationData, frameDataBuffer);
			frameData << frameDataBuffer;

			Animation::SpriteData tempSpriteData;

			while (!frameData.eof())
			{ 
				ch = frameData.get();
				if (ch == '!')
				{
					frameData >> tempSpriteData.name >> tempSpriteData.x >> tempSpriteData.y;
					frame.first.push_back(tempSpriteData);
					frame.second = "";
				}
				break;
			}

			if(ch == '!')
				animation.push_back(frame);
			else
			{
				string seName;
				frameData >> seName;
				animation[animation.size() - 1].second = seName;
			}

		}

		animationDataMap.insert(pair<string, Animation::AnimationData>(animationName, animation));
		animationData.close();

	}

	animationList.close();

}

Animation AnimationManager::getAnimation(string spName, string aniName, bool repeat)
{
	SpriteManager& sm = SpriteManager::getInstance();
	if (animationDataMap.find(aniName) == animationDataMap.end())
		return Animation();
	if (!sm.hasThisImage(spName))
		return Animation();
	Animation instance(spName, aniName);
	instance.maxFrame = animationDataMap[aniName].size();
	instance.repeat = repeat;
	return instance;
}

void Animation::draw(HDC& hdc, int x, int y, int flag)
{
	AnimationManager& am = AnimationManager::getInstance();
	SpriteManager& sm = SpriteManager::getInstance();
	FrameData frame = am.animationDataMap[animationName][curFrame];

	if (frame.second != "")
		SoundManager::getSE(frame.second)->play();

	for (int i = 0; i < frame.first.size(); i++)
	{
		Sprite sprite = sm.getSprite(spriteName, frame.first[i].name);
		sprite.draw(hdc, x + frame.first[i].x, y + frame.first[i].y, flag);
	}
}

void AnimationManager::update()
{
	list<Animation*>::iterator it;

	for (it = spriteFXList.begin(); it != spriteFXList.end(); it++)
		(*it)->update();

}

void Animation::update()
{
	if (playing)
	{
		curFrame++;
		if (curFrame >= maxFrame)
		{
			if (!repeat)
			{
				playing = false;
				curFrame = maxFrame > 0 ? maxFrame -1 : 0;
			}
			else
				curFrame = 0;
		}
	}
}