#include "common/stdafx.h"
#include "LifeComponent.h"

#include "entities/entity.h"
#include "entities/message.h"
#include "scenes/world.h"

void CLifeComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	if (m_iInvencibleTimer < m_iInvencibleTime) {
		++m_iInvencibleTimer;
	}

	if (m_iLifeTimer < m_iTimeToLive) {
		++m_iLifeTimer;
		if (m_iLifeTimer == m_iTimeToLive) {
			m_iLife = 0;
		}
	}
	if (m_iLife == 0) {
		MessageDestroy msgDestroy;
		m_owner->receiveMessage(&msgDestroy);
		m_owner->deactivate();
		g_world->removeEntity(m_owner);
	}
}

void CLifeComponent::receiveMessage(Message* _pMmessage) {
	ASSERT(_pMmessage);

	if (!m_isActive)
		return;

	MessageGetLife*msgGetLife = dynamic_cast<MessageGetLife*>(_pMmessage);
	if (msgGetLife) {
		msgGetLife->currentLife = m_iLife;

	}
	else {
		MessageChangeLife* msgChangeLife = dynamic_cast<MessageChangeLife*>(_pMmessage);
		if (msgChangeLife && (m_iLife != -1) && (m_iInvencibleTimer >= m_iInvencibleTime)) {
			m_iLife += msgChangeLife->deltaLife;
			m_iInvencibleTimer = 0;
			if (m_iLife <= 0) {
				m_iLife = 0;
			}
		}
		else {
			MessageDestroy* msgdestroy = dynamic_cast<MessageDestroy*>(_pMmessage);
			if (msgdestroy) {
				m_iLife = 0;
			}
		}
	}
}

Component* CLifeComponent::loadComponent(Entity* _pOwner, const rapidjson::Value* _pComponentInfo) {
	ASSERT(_pComponentInfo);

	// Default values
	int iLife = 1;
	int iTimeToLive = 0;
	int iInvencibleTime = 0;

	if ((*_pComponentInfo).HasMember("life")) {
		iLife = (*_pComponentInfo)["life"].GetInt();
	}

	if ((*_pComponentInfo).HasMember("timeToLive")) {
		iTimeToLive = (*_pComponentInfo)["timeToLive"].GetInt();
	}

	if ((*_pComponentInfo).HasMember("timeToLive")) {
		iInvencibleTime = (*_pComponentInfo)["invencibleTime"].GetInt();
	}

	Component* pComponent = NEW(CLifeComponent, _pOwner, iLife, iTimeToLive, iInvencibleTime);

	ASSERT(pComponent);
	pComponent->init();

	return pComponent;
}