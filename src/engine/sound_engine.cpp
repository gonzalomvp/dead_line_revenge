#include "../common/stdafx.h"
#include "sound_engine.h"

SoundEngine::SoundEngine() {
	CORE_InitSound();
}

SoundEngine::~SoundEngine() {
	for (auto itSounds= m_sounds.begin(); itSounds != m_sounds.end(); ++itSounds) {
		CORE_UnloadWav(itSounds->second);
	}
	CORE_EndSound();
}

void SoundEngine::playSound(const char* soundFile) {
	if (g_settings.sfx) {
		uint soundId = m_sounds[soundFile];
		if (!soundId) {
			soundId = CORE_LoadWav(soundFile);
			m_sounds[soundFile] = soundId;
		}
		CORE_PlayMusic(soundId);
	}
}