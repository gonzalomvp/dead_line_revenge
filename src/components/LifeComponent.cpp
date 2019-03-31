#include "common/stdafx.h"
#include "LifeComponent.h"

#include "entities/Entity.h"
#include "messages/Message.h"
#include "scenes/World.h"

void CLifeComponent::run(float _fDeltaTime) {
	CComponent::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	if (m_fInvencibleTimer > 0.0f) {
		m_fInvencibleTimer -= _fDeltaTime;
	}

	if (m_fLifeTimer > 0) {
		m_fLifeTimer -= _fDeltaTime;
		if (m_fLifeTimer <= 0) {
			onDead();
		}
	}
}

void CLifeComponent::receiveMessage(TMessage* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage);

	if (TMessageGetLife* pMessage = dynamic_cast<TMessageGetLife*>(_pMessage)) {
		pMessage->iCurrentLife = m_iLife;
	}
	else if (TMessageChangeLife* pMessage = dynamic_cast<TMessageChangeLife*>(_pMessage)) {
		// Apply heal
		if (pMessage->iDeltaLife > 0) {
			m_iLife += pMessage->iDeltaLife;
		}
		// Only apply damage if life is not infinite and not invencible
		else if (m_iLife != -1 && m_fInvencibleTimer <= 0) {
			m_iLife += pMessage->iDeltaLife;
			if (m_iLife <= 0) {
				onDead();
			}
			else {
				m_fInvencibleTimer = m_fInvencibleTime;
			}
		}
	}
	else if (TMessageDestroy* pMessage = dynamic_cast<TMessageDestroy*>(_pMessage)) {
		onDead();
	}
}

void CLifeComponent::onDead() {
	ASSERT(m_pOwner && g_pWorld);

	deactivate();
	TMessageDestroy msgDestroy;
	m_pOwner->receiveMessage(&msgDestroy);
	m_pOwner->deactivate();
	g_pWorld->removeEntity(m_pOwner);
}