#pragma once
#ifndef _CSOUND_H_
#define _CSOUND_H_

#include <fmod.h>
#include "headers.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSound 
{
private:
	static FMOD_SYSTEM *g_sound_system;

	FMOD_SOUND *m_sound;
	FMOD_CHANNEL *m_channel;

	float m_volume;
	FMOD_BOOL m_bool;
public:
	CSound(const string path, bool loop);
	~CSound();

	static int Init();
	static int Release();

	int play();
	int pause();
	int resume();
	int stop();
	int volumeUp();
	int volumeDown();

	int Update();
};

class SoundManager
{
private:
	SoundManager() {}
	SoundManager(const SoundManager& ref) {}
	SoundManager& operator=(const SoundManager& ref) { return *this; }

	static std::map<string, CSound*> bgmTable;
	static std::map<string, CSound*> seTable;

public:
	static void init();
	static CSound* getBGM(std::string name) { return bgmTable[name]; }
	static CSound* getSE(std::string name) { return seTable[name]; }

};
#endif
