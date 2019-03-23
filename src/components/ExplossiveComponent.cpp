#include "common/stdafx.h"
#include "ExplossiveComponent.h"

#include "entities/entities_factory.h"
#include "messages/message.h"
#include "scenes/world.h"

void CExplossiveComponent::receiveMessage(Message* _pMessage) {
	Component::receiveMessage(_pMessage);
	if (!m_isActive)
		return;

	ASSERT(_pMessage && m_owner && g_pWorld && g_pEntitiesFactory);
	if (MessageDestroy* pMessage = dynamic_cast<MessageDestroy*>(_pMessage)) {
		MessageGetTransform messageSelfPos;
		m_owner->receiveMessage(&messageSelfPos);
		Entity* pExplossion = g_pEntitiesFactory->createExplossion(messageSelfPos.pos, vmake(10.0f, 10.0f), vmake(2.0f, 2.0f), 50, Entity::EEXPLOSSION);
		g_pWorld->addEntity(pExplossion);
	}
}