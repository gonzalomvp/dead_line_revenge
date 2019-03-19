#pragma once

#include <map>

class SoundEngine {
public:
	SoundEngine();
	~SoundEngine();

	void playSound(const char* _psSoundFile);
	void playMusic(const char* _psSoundFile);
	void stopMusic();
	void setVolume(float _fVolume);

private:
	std::map<std::string, uint> m_mSounds;
};