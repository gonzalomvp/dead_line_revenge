#include "common/stdafx.h"
#include "LifeComponent.h"

#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"

void CLifeComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	if (m_iInvencibleTimer > 0) {
		--m_iInvencibleTimer;
	}

	if (m_iLifeTimer > 0) {
		--m_iLifeTimer;
		if (m_iLifeTimer <= 0) {
			onDead();
		}
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
		// Apply heal
		if (pMessage->deltaLife > 0) {
			m_iLife += pMessage->deltaLife;
		}
		// Only apply damage if life is not infinite and not invencible
		else if (m_iLife != -1 && m_iInvencibleTimer <= 0) {
			m_iLife += pMessage->deltaLife;
			if (m_iLife <= 0) {
				onDead();
			}
			else {
				m_iInvencibleTimer = m_iInvencibleTime;
			}
		}
	}
	else if (MessageDestroy* pMessage = dynamic_cast<MessageDestroy*>(_pMessage)) {
		onDead();
	}
}

void CLifeComponent::onDead() {
	ASSERT(m_owner && g_pWorld);

	deactivate();
	MessageDestroy msgDestroy;
	m_owner->receiveMessage(&msgDestroy);
	m_owner->deactivate();
	g_pWorld->removeEntity(m_owner);
}