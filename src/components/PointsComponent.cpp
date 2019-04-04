#include "common/stdafx.h"
#include "PointsComponent.h"

#include "messages/Message.h"
#include "scenes/World.h"

void CPointsComponent::receiveMessage(TMessage* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage && m_pOwner && g_pWorld);

	if (TMessageDestroy* pMessage = dynamic_cast<TMessageDestroy*>(_pMessage)) {
		g_pWorld->addPoints(uPoints);
	}
}