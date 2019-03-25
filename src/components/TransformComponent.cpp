#include "common/stdafx.h"
#include "TransformComponent.h"

#include "components/ColliderComponent.h"
#include "messages/message.h"
#include "scenes/world.h"

void CTransformComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	m_v2Size = vadd(m_v2Size, m_v2SizeIncrement);
}

void CTransformComponent::receiveMessage(Message* _pMessage) {
	Component::receiveMessage(_pMessage);
	if (!m_isActive)
		return;

	ASSERT(_pMessage && m_owner);

	//if (MessageSetTransform* pMessage = dynamic_cast<MessageSetTransform*>(_pMessage)) {
	//	m_v2Pos = pMessage->pos;
	//	m_v2Size = pMessage->size;

	//	m_v2Pos.x = clamp(pMessage->pos.x, m_v2Size.x * 0.5f, WORLD_WIDTH  - m_v2Size.x * 0.5f);
	//	m_v2Pos.y = clamp(pMessage->pos.y, m_v2Size.y * 0.5f, WORLD_HEIGHT - m_v2Size.y * 0.5f);

	//	if (m_v2Pos.x != pMessage->pos.x || m_v2Pos.y != pMessage->pos.y) {
	//		MessageCheckCollision msgCheckCollision;
	//		msgCheckCollision.overlap = true;
	//		msgCheckCollision.deltaLife = -1;
	//		msgCheckCollision.collisionChannel = CColliderComponent::EBoundariesCollider;
	//		msgCheckCollision.bounceX = m_v2Pos.x != pMessage->pos.x;
	//		msgCheckCollision.bounceY = m_v2Pos.y != pMessage->pos.y;
	//		m_owner->receiveMessage(&msgCheckCollision);
	//	}
	//}
	//else if(MessageGetTransform* pMessage = dynamic_cast<MessageGetTransform*>(_pMessage)) {
	//	pMessage->pos = m_v2Pos;
	//	pMessage->size = m_v2Size;
	//}
}
