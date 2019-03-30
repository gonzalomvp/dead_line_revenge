#include "common/stdafx.h"
#include "AIFireComponent.h"

#include "messages/message.h"
#include "scenes/world.h"

#include <algorithm>

void CAIFireComponent::init() {
	Component::init();

	if (m_vAimDirections.size() > 0) {
		if (m_bWantShuffle) {
			std::random_shuffle(m_vAimDirections.begin(), m_vAimDirections.end());
		}
		m_uCurrentAimDirectionIndex = 0;
		m_v2AimDirection = m_vAimDirections[m_uCurrentAimDirectionIndex];
	}
}

void CAIFireComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	//ASSERT(m_pOwner && g_pWorld && g_pWorld->getPlayer());

	MessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.direction = m_v2AimDirection;
	//messageSetAimDirection.changeAngle = true;
	m_pOwner->receiveMessage(&messageSetAimDirection);

	////MessageFire msgFire;
	////msgFire.isFiring = true;
	////m_pOwner->receiveMessage(&msgFire);

	//if (m_vFireDirections.size() > 0) {
	//	MessageSetAimDirection messageSetAimDirection;
	//	messageSetAimDirection.direction = m_vFireDirections[m_uCurrentFireDirectionIndex];
	//	messageSetAimDirection.changeAngle = true;
	//	m_pOwner->receiveMessage(&messageSetAimDirection);
	//}
	//else {
	//	MessageGetTransform messageSelfPos;
	//	m_pOwner->receiveMessage(&messageSelfPos);
	//	MessageGetTransform messagePlayerPos;
	//	g_pWorld->getPlayer()->receiveMessage(&messagePlayerPos);
	//	MessageSetAimDirection messageSetAimDirection;
	//	messageSetAimDirection.direction = vnorm(vsub(messagePlayerPos.pos, messageSelfPos.pos));
	//	messageSetAimDirection.changeAngle = true;
	//	m_pOwner->receiveMessage(&messageSetAimDirection);
	//}
}

void CAIFireComponent::receiveMessage(Message* _pMessage) {
	CAIComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage);

	if (MessageSetAimDirection* pMessage = dynamic_cast<MessageSetAimDirection*>(_pMessage)) {
		if (vlen2(pMessage->direction) != 0.0f) {
			m_v2AimDirection = pMessage->direction;
		}
		else if (m_vAimDirections.size() > 0) {
			++m_uCurrentAimDirectionIndex;
			if (m_uCurrentAimDirectionIndex >= m_vAimDirections.size()) {
				m_uCurrentAimDirectionIndex = 0;
			}
			m_v2AimDirection = m_vAimDirections[m_uCurrentAimDirectionIndex];
		}
	}
	else if (MessageGetAimDirection* pMessage = dynamic_cast<MessageGetAimDirection*>(_pMessage)) {
		pMessage->direction = m_v2AimDirection;
	}
}