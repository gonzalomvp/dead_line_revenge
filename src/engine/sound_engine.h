#pragma once

#include "common/stdafx.h"
#include <map>

class SoundEngine {
public:
	SoundEngine();
	~SoundEngine();

	void playSound(const char* soundFile);
	void playMusic(const char* soundFile);
	void stopMusic();
	void setVolume(float volume);

private:
	std::map<std::string, uint> m_sounds;
};