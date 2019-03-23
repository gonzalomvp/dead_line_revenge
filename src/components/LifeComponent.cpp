#include "common/stdafx.h"
#include "LifeComponent.h"

#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"

void CLifeComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	ASSERT(m_owner && g_pWorld);

	if (m_iInvencibleTimer > 0) {
		--m_iInvencibleTimer;
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

void CLifeComponent::receiveMessage(Message* _pMessage) {
	Component::receiveMessage(_pMessage);
	if (!m_isActive)
		return;

	ASSERT(_pMessage);

	if (MessageGetLife* pMessage = dynamic_cast<MessageGetLife*>(_pMessage)) {
		pMessage->currentLife = m_iLife;
	}
	else if (MessageChangeLife* pMessage = dynamic_cast<MessageChangeLife*>(_pMessage)) {
		if ((m_iLife != -1) && (m_iInvencibleTimer <= 0)) {
			m_iLife += pMessage->deltaLife;
			m_iInvencibleTimer = m_iInvencibleTime;
			if (m_iLife <= 0) {
				m_iLife = 0;
			}
		}
	}
	else if (MessageDestroy* pMessage = dynamic_cast<MessageDestroy*>(_pMessage)) {
		m_iLife = 0;
	}
}
