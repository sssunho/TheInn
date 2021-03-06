#include "sprite.h"
#include "wndControl.h"
#include <vector>
#include <fstream>

using namespace std;

extern RECT clientRect;

SpriteManager::SpriteManager()
{
	int n;
	fstream atlasList;
	string atlasName;
	Image* atlasImage;

	atlasList.open("sprite/list.txt", ios::in);
	if (!atlasList.is_open())
		return;
	atlasList >> n;
	for (int i = 0; i < n; i++)
	{
		wstring imagePath;
		fstream spriteList;
		atlasList >> atlasName;
		imagePath.assign(atlasName.begin(), atlasName.end());
		wprintf_s(imagePath.c_str());

		imagePath = L"sprite/" + imagePath + L".png";
		atlasImage = Image::FromFile(imagePath.c_str());

		if (atlasImage->GetLastStatus() != Ok)
			continue;

		imageMap.insert(pair<string, Image*>(atlasName, atlasImage));

		spriteList.open("sprite/" + atlasName + ".txt", ios::in);
		if (!spriteList.is_open())
			continue;

		map<string, Rect> spriteMap;

		while (!spriteList.eof())
		{
			string spriteName;
			Rect spriteData;

			spriteList >> spriteName >> spriteData.X >> spriteData.Y >> spriteData.Width >> spriteData.Height;

			spriteMap.insert(pair<string, Rect>(spriteName, spriteData));

		}

		dataMap.insert(pair<string, map<string, Rect>>(atlasName, spriteMap));

		spriteList.close();
	}

	vector<wstring> tilesetImgList;
	GetFiles(tilesetImgList, L"Graphics\\Tilesets", false);
	for (int i = 0; i < tilesetImgList.size(); i++)
	{
		Image* tilesetImg = Image::FromFile(tilesetImgList[i].c_str());
		string multiByteName;
		multiByteName.assign(tilesetImgList[i].begin(), tilesetImgList[i].end());
		tilesetMap.insert(pair<string, Image*>(
			string(multiByteName.c_str() + strlen("Graphics\\Tilesets") + 1),
			tilesetImg)
		);
	}

	vector<wstring> autotileImgList;
	GetFiles(autotileImgList, L"Graphics\\Autotiles", false);
	for (int i = 0; i < autotileImgList.size(); i++)
	{
		Image* autotileImg = Image::FromFile(autotileImgList[i].c_str());
		string multiByteName;
		multiByteName.assign(autotileImgList[i].begin(), autotileImgList[i].end());
		autotileMap.insert(pair<string, Image*>(
			string(multiByteName.c_str() + strlen("Graphics\\Autotiles") + 1),
			autotileImg)
		);
	}
	atlasList.close();
}

void SpriteManager::loadImage(std::wstring path, std::string name)
{
	Image* image = Image::FromFile(path.c_str());
	imageMap.insert(pair<string, Image*>(name, image));
}

void SpriteManager::draw(const HDC & hdc, string atlas, int x, int y, string sprite)
{

	if (imageMap.find(atlas) == imageMap.end())
		return;

	Graphics g(hdc);
	Image* sourceImage = imageMap[atlas];
	Rect sourceData;
	Rect dest;

	if (dataMap.find(sprite) != dataMap.end() || sprite == "")
	{
		sourceData = { 0, 0, (int)sourceImage->GetWidth(), (int)sourceImage->GetHeight() };
	}
	else
	{
		sourceData = dataMap[atlas][sprite + ".png"];
	}

	dest = sourceData;
	dest.X = x - sourceData.Width / 2; 
	dest.Y = y - sourceData.Height / 2;

	g.DrawImage(imageMap[atlas], dest, sourceData.X, sourceData.Y, sourceData.Width, sourceData.Height, UnitPixel);

}

Sprite SpriteManager::getSprite(string imageFile, string subSprite)
{
	Sprite sprite;

	if (imageMap.find(imageFile) == imageMap.end())
		return sprite;

	sprite.img = imageMap[imageFile];

	if (dataMap.find(imageFile) != dataMap.end())
	{
		subSprite += ".png";
		sprite.cx = dataMap[imageFile][subSprite].X;
		sprite.cy = dataMap[imageFile][subSprite].Y;
		sprite.width = dataMap[imageFile][subSprite].Width;
		sprite.height = dataMap[imageFile][subSprite].Height;
	}
	else
	{
		sprite.cx = 0;
		sprite.cy = 0;
		sprite.width = sprite.img->GetWidth();
		sprite.height = sprite.img->GetHeight();
	}

	return sprite;
}

bool SpriteManager::hasThisImage(string imageName)
{
	return imageMap.find(imageName) != imageMap.end();
}

void Sprite::draw(const HDC& hdc, int x, int y, int flag)
{
	if (img == NULL)
		return;
	Graphics g(hdc);
	Rect dest = { x - width / 2, y - height / 2, width, height };
	if (flag)
	{
		if (flag & TF_XFLIP)
		{
			Matrix transformMat = { 1.0f, 0.0f, 0.0f, -1.0f,
				0 , (float)(2 * dest.Y + height) };
			g.SetTransform(&transformMat);
			
		}
		if (flag & TF_YFLIP)
		{
			Matrix transformMat = { -1.0f, 0.0f, 0.0f, 1.0f,
				(float)(2 * dest.X + width),0 };
			g.SetTransform(&transformMat);
		}
		if (flag & TF_XYFLIP)
		{
			Matrix transformMat = { -1.0f, 0.0f, 0.0f, -1.0f,
				(float)(2 * dest.X + width),(float)(2 * dest.Y + height) };
			g.SetTransform(&transformMat);
		}
	}
	g.DrawImage(img, dest, cx, cy, width, height, UnitPixel);
}

void Sprite::draw(const HDC & hdc, int x, int y, float rx, float ry)
{
	if (img == NULL)
		return;
	Graphics g(hdc);
	Rect dest = { x - width / 2, y - height / 2, (int)round(width * rx), (int)round(height * ry) };
	g.DrawImage(img, dest, cx, cy, width, height, UnitPixel);
}
