#include "common/stdafx.h"
#include "PointsComponent.h"

#include "messages/message.h"
#include "scenes/world.h"

void CPointsComponent::receiveMessage(Message* _pMessage) {
	Component::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage && m_pOwner && g_pWorld);

	if (MessageDestroy* pMessage = dynamic_cast<MessageDestroy*>(_pMessage)) {
		g_pWorld->addPoints(m_pOwner->getType());
	}
}