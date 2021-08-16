#include "sound.h"
#include "framework.h"
#include <fmod.h>

FMOD_SYSTEM* CSound::g_sound_system;
map<string, CSound*> SoundManager::bgmTable;
map<string, CSound*> SoundManager::seTable;

CSound* SoundManager::loadedBgm;
std::list<CSound*> SoundManager::seList;

CSound::CSound(const string path, bool loop) {
	if (loop) {
		FMOD_System_CreateSound(g_sound_system, path.c_str(), FMOD_LOOP_NORMAL, 0, &m_sound);
	}
	else {
		FMOD_System_CreateSound(g_sound_system, path.c_str(), FMOD_DEFAULT, 0, &m_sound);
	}

	m_channel = nullptr;
	m_volume = SOUND_DEFAULT;
}

CSound::~CSound() {
	FMOD_Sound_Release(m_sound);
}


int CSound::Init() {
	FMOD_System_Create(&g_sound_system);
	FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, NULL);

	return 0;
}

int CSound::Release() {
	FMOD_System_Close(g_sound_system);
	FMOD_System_Release(g_sound_system);

	return 0;
}


int CSound::play() {
	FMOD_System_PlaySound(g_sound_system, m_sound, NULL, false, &m_channel);

	return 0;
}

int CSound::pause() {
	FMOD_Channel_SetPaused(m_channel, true);

	return 0;
}

int CSound::resume() {
	FMOD_Channel_SetPaused(m_channel, false);

	return 0;
}

int CSound::stop() {
	FMOD_Channel_Stop(m_channel);

	return 0;
}

int CSound::volumeUp() {
	if (m_volume < SOUND_MAX) {
		m_volume += SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(m_channel, m_volume);

	return 0;
}

int CSound::volumeDown() {
	if (m_volume > SOUND_MIN) {
		m_volume -= SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(m_channel, m_volume);

	return 0;
}

bool CSound::isPlaying()
{
	FMOD_BOOL playing;
	FMOD_Channel_IsPlaying(m_channel, &playing);
	return playing;
}


int CSound::Update() {
	FMOD_Channel_IsPlaying(m_channel, &m_bool);

	if (m_bool) {
		FMOD_System_Update(g_sound_system);
	}

	return 0;
}

void SoundManager::update()
{
	list<CSound*>::iterator it;
	for (it = seList.begin(); it != seList.end(); )
	{
		if ((*it)->isPlaying())
		{
			(*it)->Update();
			it++;
		}
		else
		{
			delete *it;
			it = seList.erase(it);
		}
	}
	if (loadedBgm)
		loadedBgm->Update();
}

void SoundManager::playSE(string name)
{
	CSound* inst = new CSound("Audio\\SE\\" + name, false);
	inst->play();
	seList.push_back(inst);
}

void SoundManager::loadBGM(string name)
{
	if (loadedBgm)
		releaseBGM();
	loadedBgm = new CSound("Audio\\BGM\\" + name, true);
}

void SoundManager::playBGM()
{
	if (loadedBgm)
		loadedBgm->play();
}

void SoundManager::pauseBGM()
{
	if (loadedBgm)
		loadedBgm->pause();
}

void SoundManager::releaseBGM()
{
	if (loadedBgm)
		delete loadedBgm;
	loadedBgm = nullptr;
}
