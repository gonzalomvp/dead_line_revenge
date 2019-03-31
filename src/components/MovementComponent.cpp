#include "common/stdafx.h"
#include "MovementComponent.h"

#include "entities/Entity.h"
#include "messages/Message.h"
#include "scenes/World.h"

void CMovementComponent::run(float _fDeltaTime) {
	CComponent::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	ASSERT(m_pOwner);

	if (vlen2(m_v2Direction) != 0.0f) {
		vec2 v2NewPos = vadd(m_pOwner->getPos(), vscale(vnorm(m_v2Direction), m_fSpeed * _fDeltaTime));
		vec2 v2Size = m_pOwner->getSize();

		vec2 v2ValidPos = v2NewPos;
		v2ValidPos.x = clamp(v2ValidPos.x, v2Size.x * 0.5f, WORLD_WIDTH - v2Size.x * 0.5f);
		v2ValidPos.y = clamp(v2ValidPos.y, v2Size.y * 0.5f, WORLD_HEIGHT - v2Size.y * 0.5f);

		if (v2ValidPos.x != v2NewPos.x || v2ValidPos.y != v2NewPos.y) {
			TMessageCheckCollision msgCheckCollision;
			msgCheckCollision.overlap = true;
			msgCheckCollision.deltaLife = -1;
			msgCheckCollision.collisionChannel = CColliderComponent::EBoundariesCollider;
			m_pOwner->receiveMessage(&msgCheckCollision);

			if (m_bHasBounce && v2ValidPos.x != v2NewPos.x) {
				m_v2Direction.x = -m_v2Direction.x;
			}
			if (m_bHasBounce && v2ValidPos.y != v2NewPos.y) {
				m_v2Direction.y = -m_v2Direction.y;
			}
		}
		m_pOwner->setPos(v2ValidPos);
	}
}

void CMovementComponent::receiveMessage(TMessage* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage);

	if (TMessageGetMovementDir* pMessage = dynamic_cast<TMessageGetMovementDir*>(_pMessage)) {
		pMessage->dir = m_v2Direction;
	}
	else if (TMessageSetMovementDir* pMessage = dynamic_cast<TMessageSetMovementDir*>(_pMessage)) {
		m_v2Direction = pMessage->dir;
	}
	else if (TMessageSetMovementSpeed* pMessage = dynamic_cast<TMessageSetMovementSpeed*>(_pMessage)) {
		m_fSpeed = pMessage->speed;
	}
}
