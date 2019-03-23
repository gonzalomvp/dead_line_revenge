#include "common/stdafx.h"
#include "AIFireComponent.h"

#include "messages/message.h"
#include "scenes/world.h"

#include <algorithm>

void CAIFireComponent::init() {
	Component::init();

	if (m_vFireDirections.size() > 0 && m_bWantShuffle) {
		std::random_shuffle(m_vFireDirections.begin(), m_vFireDirections.end());
	}
}

void CAIFireComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	ASSERT(m_owner && g_pWorld && g_pWorld->getPlayer());

	MessageFire msgFire;
	msgFire.isFiring = true;
	m_owner->receiveMessage(&msgFire);

	if (m_vFireDirections.size() > 0) {
		MessageSetAimDirection messageSetAimDirection;
		messageSetAimDirection.direction = m_vFireDirections[m_uCurrentFireDirectionIndex];
		messageSetAimDirection.changeAngle = true;
		m_owner->receiveMessage(&messageSetAimDirection);
	}
	else {
		MessageGetTransform messageSelfPos;
		m_owner->receiveMessage(&messageSelfPos);
		MessageGetTransform messagePlayerPos;
		g_pWorld->getPlayer()->receiveMessage(&messagePlayerPos);
		MessageSetAimDirection messageSetAimDirection;
		messageSetAimDirection.direction = vnorm(vsub(messagePlayerPos.pos, messageSelfPos.pos));
		messageSetAimDirection.changeAngle = true;
		m_owner->receiveMessage(&messageSetAimDirection);
	}
}

void CAIFireComponent::receiveMessage(Message* _pMessage) {
	CAIComponent::receiveMessage(_pMessage);
	if (!m_isActive)
		return;

	ASSERT(_pMessage);

	if (MessageFire* pMessage = dynamic_cast<MessageFire*>(_pMessage)) {
		if (pMessage->isFireDone && m_vFireDirections.size() > 0) {
			++m_uCurrentFireDirectionIndex;
			if (m_uCurrentFireDirectionIndex >= m_vFireDirections.size()) {
				m_uCurrentFireDirectionIndex = 0;
			}
		}
	}
}