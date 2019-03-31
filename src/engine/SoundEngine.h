#pragma once

#include <map>

class CSoundEngine {
public:
	CSoundEngine();
	~CSoundEngine();

	void playSound(const char* _psSoundFile);
	void playMusic(const char* _psSoundFile);
	void stopMusic();
	void setVolume(float _fVolume);

private:
	std::map<std::string, uint> m_mSounds;
};