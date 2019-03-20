#include "common/stdafx.h"
#include "LifeComponent.h"

#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"

void CLifeComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	ASSERT(m_owner);

	if (m_iInvencibleTimer < m_iInvencibleTime) {
		++m_iInvencibleTimer;
	}

	if (m_iLifeTimer < m_iTimeToLive) {
		++m_iLifeTimer;
		if (m_iLifeTimer == m_iTimeToLive) {
			m_iLife = 0;
		}
	}
	if (m_iLife == 0) {
		MessageDestroy msgDestroy;
		m_owner->receiveMessage(&msgDestroy);
		m_owner->deactivate();
		g_pWorld->removeEntity(m_owner);
	}
}

void CLifeComponent::receiveMessage(Message* _pMmessage) {
	ASSERT(_pMmessage);

	if (!m_isActive)
		return;

	if (MessageGetLife* pMessage = dynamic_cast<MessageGetLife*>(_pMmessage)) {
		pMessage->currentLife = m_iLife;
	}
	else if (MessageChangeLife* pMessage = dynamic_cast<MessageChangeLife*>(_pMmessage)) {
		if ((m_iLife != -1) && (m_iInvencibleTimer >= m_iInvencibleTime)) {
			m_iLife += pMessage->deltaLife;
			m_iInvencibleTimer = 0;
			if (m_iLife <= 0) {
				m_iLife = 0;
			}
		}
	}
	else if (MessageDestroy* pMessage = dynamic_cast<MessageDestroy*>(_pMmessage)) {
		m_iLife = 0;
	}
}
