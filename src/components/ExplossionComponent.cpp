#include "common/stdafx.h"
#include "ExplossionComponent.h"

#include "entities/Entity.h"
#include "engine/SoundEngine.h"

namespace {
	const std::string s_sExplossionSoundFile = "data/audio/explossion.wav";
}

void CExplossionComponent::run(float _fDeltaTime) {
	CComponent::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	ASSERT(m_pOwner && g_pSoundEngine);

	if (m_fExplossionTimer >= 0.0f) {
		float fFactor = clamp(m_fExplossionTimer / m_fExplossionTime, 0.0f, 1.0f);

		// Play sound only first time
		if (fFactor >= 1.0f) {
			g_pSoundEngine->playSound(s_sExplossionSoundFile.c_str());
		}

		vec2 v2NewSize = vmake(0.0f, 0.0f);
		v2NewSize.x = m_v2EndSize.x + (m_v2InitSize.x - m_v2EndSize.x) * fFactor;
		v2NewSize.y = m_v2EndSize.y + (m_v2InitSize.y - m_v2EndSize.y) * fFactor;

		m_pOwner->setSize(v2NewSize);
		m_fExplossionTimer -= _fDeltaTime;
	}
}