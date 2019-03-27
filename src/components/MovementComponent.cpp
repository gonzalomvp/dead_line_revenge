#include "common/stdafx.h"
#include "MovementComponent.h"

#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"

void CMovementComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	ASSERT(m_owner);

	if (vlen2(m_v2Direction) != 0.0f) {
		vec2 v2NewPos = vadd(m_owner->getPos(), vscale(vnorm(m_v2Direction), m_fSpeed * _fDeltaTime));
		vec2 v2Size = m_owner->getSize();

		vec2 v2ValidPos = v2NewPos;
		v2ValidPos.x = clamp(v2ValidPos.x, v2Size.x * 0.5f, WORLD_WIDTH - v2Size.x * 0.5f);
		v2ValidPos.y = clamp(v2ValidPos.y, v2Size.y * 0.5f, WORLD_HEIGHT - v2Size.y * 0.5f);

		if (v2ValidPos.x != v2NewPos.x || v2ValidPos.y != v2NewPos.y) {
			MessageCheckCollision msgCheckCollision;
			msgCheckCollision.overlap = true;
			msgCheckCollision.deltaLife = -1;
			msgCheckCollision.collisionChannel = CColliderComponent::EBoundariesCollider;
			m_owner->receiveMessage(&msgCheckCollision);

			if (m_bHasBounce && v2ValidPos.x != v2NewPos.x) {
				m_v2Direction.x = -m_v2Direction.x;
			}
			if (m_bHasBounce && v2ValidPos.y != v2NewPos.y) {
				m_v2Direction.y = -m_v2Direction.y;
			}
		}
		m_owner->setPos(v2ValidPos);
	}
}

void CMovementComponent::receiveMessage(Message* _pMessage) {
	Component::receiveMessage(_pMessage);
	if (!m_isActive)
		return;

	ASSERT(_pMessage);

	if (MessageGetMovementDir* pMessage = dynamic_cast<MessageGetMovementDir*>(_pMessage)) {
		pMessage->dir = m_v2Direction;
	}
	else if (MessageSetMovementDir* pMessage = dynamic_cast<MessageSetMovementDir*>(_pMessage)) {
		m_v2Direction = pMessage->dir;
	}
	else if (MessageSetMovementSpeed* pMessage = dynamic_cast<MessageSetMovementSpeed*>(_pMessage)) {
		m_fSpeed = pMessage->speed;
	}
}
