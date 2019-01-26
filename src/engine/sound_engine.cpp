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
		CORE_PlaySound(soundId, g_settings.volume, 1.0f);
	}
}

void SoundEngine::playMusic(const char* soundFile) {
	if (g_settings.music) {
		uint soundId = m_sounds[soundFile];
		if (!soundId) {
			soundId = CORE_LoadWav(soundFile);
			m_sounds[soundFile] = soundId;
		}
		CORE_PlayMusic(soundId, true, g_settings.volume);
	}
}

void SoundEngine::stopMusic() {
	CORE_StopMusic();
}

void SoundEngine::setVolume(float volume) {
	CORE_SetMusicVolume(volume);
}