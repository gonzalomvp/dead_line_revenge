#include "common/stdafx.h"
#include "ExplossiveComponent.h"

#include "entities/EntitiesFactory.h"
#include "messages/Message.h"
#include "scenes/World.h"

void CExplossiveComponent::receiveMessage(TMessage* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage && m_pOwner && g_pWorld && g_pEntitiesFactory);
	if (TMessageDestroy* pMessage = dynamic_cast<TMessageDestroy*>(_pMessage)) {
		CEntity* pExplossion = g_pEntitiesFactory->createExplossion(m_pOwner->getPos());
		g_pWorld->addEntity(pExplossion);
	}
}