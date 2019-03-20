#include "common/stdafx.h"
#include "TransformComponent.h"

#include "messages/message.h"
#include "scenes/world.h"

void CTransformComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	m_v2Size = vadd(m_v2Size, m_v2SizeIncrement);
}

void CTransformComponent::receiveMessage(Message* _pMessage) {
	ASSERT(_pMessage);

	if (!m_isActive)
		return;

	if (MessageSetTransform* pMessage = dynamic_cast<MessageSetTransform*>(_pMessage)) {
		m_v2Pos = pMessage->pos;
		m_v2Size = pMessage->size;
		bool bIsOutOfBounds = false;

		m_v2Pos.x = clamp(pMessage->pos.x, m_v2Size.x * 0.5f, WORLD_WIDTH  - m_v2Size.x * 0.5f);
		m_v2Pos.y = clamp(pMessage->pos.y, m_v2Size.y * 0.5f, WORLD_HEIGHT - m_v2Size.y * 0.5f);

		if (m_v2Pos.x != pMessage->pos.x || m_v2Pos.y != pMessage->pos.y) {
			MessageCheckCollision msgCheckCollision;
			msgCheckCollision.overlap = true;
			msgCheckCollision.deltaLife = -1;
			msgCheckCollision.collisionChannel = ComponentCollider::EBoundariesCollider;
			msgCheckCollision.bounceX = m_v2Pos.x != pMessage->pos.x;
			msgCheckCollision.bounceY = m_v2Pos.y != pMessage->pos.y;
			m_owner->receiveMessage(&msgCheckCollision);
		}

		/*if (m_v2Pos.x > SCR_WIDTH - m_v2Size.x * 0.5f) {
			m_v2Pos.x = WORLD_WIDTH - m_v2Size.x * 0.5f;
			bIsOutOfBounds = true;
			v2BounceDirection = vmake(-1.0f, 1.0f);
		}
		else if (m_v2Pos.x < 0 + m_v2Size.x * 0.5f) {
			m_v2Pos.x = 0 + m_v2Size.x * 0.5f;
			bIsOutOfBounds = true;
			v2BounceDirection = vmake(-1.0f, 1.0f);
		}
		if (m_v2Pos.y > WORLD_HEIGHT - m_v2Size.y * 0.5f) {
			m_v2Pos.y = WORLD_HEIGHT - m_v2Size.y * 0.5f;
			bIsOutOfBounds = true;
			v2BounceDirection = vmake(1.0f, -1.0f);
		}
		else if (m_v2Pos.y < 0 + m_v2Size.y * 0.5f) {
			m_v2Pos.y = 0 + m_v2Size.y * 0.5f;
			bIsOutOfBounds = true;
			v2BounceDirection = vmake(1.0f, -1.0f);
		}*/

		/*if (bIsOutOfBounds) {
			
		}*/
	}
	else if(MessageGetTransform* pMessage = dynamic_cast<MessageGetTransform*>(_pMessage)) {
		pMessage->pos = m_v2Pos;
		pMessage->size = m_v2Size;
	}
}
