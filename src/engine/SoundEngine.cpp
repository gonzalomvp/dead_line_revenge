#include "common/stdafx.h"
#include "SoundEngine.h"

CSoundEngine::CSoundEngine() {
	CORE_InitSound();
}

CSoundEngine::~CSoundEngine() {
	for (auto itSounds= m_mSounds.begin(); itSounds != m_mSounds.end(); ++itSounds) {
		CORE_UnloadWav(itSounds->second);
	}
	CORE_EndSound();
}

void CSoundEngine::playSound(const char* _psSoundFile) {
	if (g_settings.sfx) {
		uint uSoundId = UINT_MAX;
		if (m_mSounds.count(_psSoundFile)) {
			uSoundId = m_mSounds[_psSoundFile];
		}
		else {
			uSoundId = CORE_LoadWav(_psSoundFile);
			m_mSounds[_psSoundFile] = uSoundId;
		}
		ASSERT(uSoundId != UINT_MAX, "Audio %s not found", _psSoundFile);
		CORE_PlaySound(uSoundId, g_settings.volume, 1.0f);
	}
}

void CSoundEngine::playMusic(const char* _psSoundFile) {
	if (g_settings.music) {
		uint uSoundId = UINT_MAX;
		if (m_mSounds.count(_psSoundFile)) {
			uSoundId = m_mSounds[_psSoundFile];
		}
		else {
			uSoundId = CORE_LoadWav(_psSoundFile);
			m_mSounds[_psSoundFile] = uSoundId;
		}
		ASSERT(uSoundId != UINT_MAX, "Audio %s not found", _psSoundFile);
		CORE_PlayMusic(uSoundId, true, g_settings.volume);
	}
}

void CSoundEngine::stopMusic() {
	CORE_StopMusic();
}

void CSoundEngine::setVolume(float _fVolume) {
	CORE_SetMusicVolume(_fVolume);
}