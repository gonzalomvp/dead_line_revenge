#include "common/stdafx.h"
#include "ExplossiveComponent.h"

#include "entities/entities_factory.h"
#include "messages/message.h"
#include "scenes/world.h"

void CExplossiveComponent::receiveMessage(Message* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage && m_pOwner && g_pWorld && g_pEntitiesFactory);
	if (MessageDestroy* pMessage = dynamic_cast<MessageDestroy*>(_pMessage)) {
		Entity* pExplossion = g_pEntitiesFactory->createExplossion(m_pOwner->getPos());
		g_pWorld->addEntity(pExplossion);
	}
}