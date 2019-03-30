#include "common/stdafx.h"
#include "LifeComponent.h"

#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"

void CLifeComponent::run(float _fDeltaTime) {
	CComponent::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	if (m_iInvencibleTimer > 0) {
		--m_iInvencibleTimer;
	}

	if (m_fLifeTimer > 0) {
		m_fLifeTimer -= _fDeltaTime;
		if (m_fLifeTimer <= 0) {
			onDead();
		}
	}
}

void CLifeComponent::receiveMessage(Message* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
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
	ASSERT(m_pOwner && g_pWorld);

	deactivate();
	MessageDestroy msgDestroy;
	m_pOwner->receiveMessage(&msgDestroy);
	m_pOwner->deactivate();
	g_pWorld->removeEntity(m_pOwner);
}