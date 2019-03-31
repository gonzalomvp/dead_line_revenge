#include "common/stdafx.h"
#include "entity.h"

#include"components/Component.h"

#include <algorithm>

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