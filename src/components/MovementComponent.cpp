#include "common/stdafx.h"
#include "MovementComponent.h"

#include "entities/entity.h"
#include "messages/message.h"

void CMovementComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	ASSERT(m_owner);

	if (vlen2(m_v2Direction) != 0) {
		MessageGetTransform msgGetTransform;
		m_owner->receiveMessage(&msgGetTransform);

		MessageSetTransform msgSetTransform;
		msgSetTransform.pos = vadd(msgGetTransform.pos, vscale(vnorm(m_v2Direction), m_fSpeed));
		msgSetTransform.size = msgGetTransform.size;
		m_owner->receiveMessage(&msgSetTransform);
	}

	if (!m_bHasInertia) {
		m_v2Direction = vmake(0.0f, 0.0f);
	}
}

void CMovementComponent::receiveMessage(Message* _pMessage) {
	Component::receiveMessage(_pMessage);
	if (!m_isActive)
		return;

	ASSERT(_pMessage && m_owner);

	if (MessageAddMovement* pMessage = dynamic_cast<MessageAddMovement*>(_pMessage)) {
		m_v2Direction = vadd(m_v2Direction, pMessage->dir);
	}
	else if (MessageCheckCollision* pMessage = dynamic_cast<MessageCheckCollision*>(_pMessage)) {
		if (pMessage->overlap && m_bHasBounce) {
			if (pMessage->bounceX) {
				m_v2Direction.x = -m_v2Direction.x;
			}
			if (pMessage->bounceY) {
				m_v2Direction.y = -m_v2Direction.y;
			}
			MessageSetAimDirection messageSetAimDirection;
			messageSetAimDirection.changeAngle = true;
			messageSetAimDirection.direction = m_v2Direction;
			m_owner->receiveMessage(&messageSetAimDirection);
		}
	}
}
