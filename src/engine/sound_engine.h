#pragma once

#include "../common/stdafx.h"
#include <map>

class SoundEngine {
public:
	SoundEngine();
	~SoundEngine();

	void playSound(const char* soundFile);

private:
	std::map<std::string, uint> m_sounds;
};