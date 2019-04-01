#include "common/stdafx.h"
#include "entity.h"

#include"components/Component.h"

#include <algorithm>

CEntity::TEntityInfo CEntity::s_aEntityInfo[] = {
#define REG_ENTITY(val, name) \
	{CEntity::E##val, name},
#include "REG_ENTITIES.h"
#undef REG_ENTITY
};

CEntity::EType CEntity::getEntityTypeByName(const std::string& name) {
	CEntity::EType etype = CEntity::EInvalid;
	int i = 0;
	while ((etype == CEntity::EInvalid) && (i < CEntity::NUM_ENTITIES)) {
		if (name == s_aEntityInfo[i].sName) {
			etype = s_aEntityInfo[i].eType;
		}
		i++;
	}
	return etype;
}

CEntity::~CEntity() {
	for (auto itComponent = m_vComponents.begin(); itComponent != m_vComponents.end(); ++itComponent) {
		DELETE(*itComponent);
	}
	for (auto itDestroyListener = m_vDestroyListeners.begin(); itDestroyListener != m_vDestroyListeners.end(); ++itDestroyListener) {
		(*itDestroyListener)->onEntityDestroyed(this);
	}
}

void CEntity::activate() {
	for (auto itComponent = m_vComponents.begin(); itComponent != m_vComponents.end(); ++itComponent) {
		(*itComponent)->activate();
	}
}

void CEntity::deactivate() {
	for (auto itComponent = m_vComponents.begin(); itComponent != m_vComponents.end(); ++itComponent) {
		(*itComponent)->deactivate();
	}
}

void CEntity::run(float _fDeltaTime) {
	for (auto itComponent = m_vComponents.begin(); itComponent != m_vComponents.end(); ++itComponent) {
		(*itComponent)->run(_fDeltaTime);
	}
}

void CEntity::receiveMessage(TMessage* _pMessage) {
	for (auto itComponent = m_vComponents.begin(); itComponent != m_vComponents.end(); ++itComponent) {
		(*itComponent)->receiveMessage(_pMessage);
	}
}

void CEntity::addComponent(CComponent* _pComponent) {
	m_vComponents.push_back(_pComponent);
}

void CEntity::registerToDestroy(IListener* _pListener) {
	m_vDestroyListeners.push_back(_pListener);
}

void CEntity::unregisterToDestroy(IListener* _pListener) {
	m_vDestroyListeners.erase(
		std::remove(m_vDestroyListeners.begin(), m_vDestroyListeners.end(), _pListener),
		m_vDestroyListeners.end()
	);
}