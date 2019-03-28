#include "common/stdafx.h"
#include "entity.h"

#include"components/component.h"

#include <algorithm>

Entity::~Entity() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		DELETE(*itComponents);
	}
	for (auto itDestroyListener = m_vlisteners.begin(); itDestroyListener != m_vlisteners.end(); ++itDestroyListener) {
		(*itDestroyListener)->onEntityDestroyed(this);
	}
}

void Entity::activate() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->activate();
	}
}

void Entity::deactivate() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->deactivate();
	}
}

void Entity::run(float deltaTime) {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->run(deltaTime);
	}
}

void Entity::receiveMessage(Message* message) {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->receiveMessage(message);
	}
}

void Entity::addComponent(Component* component) {
	m_components.push_back(component);
}

void Entity::registerToDestroy(IListener* _pListener) {
	m_vlisteners.push_back(_pListener);
}

void Entity::unregisterToDestroy(IListener* _pListener) {
	m_vlisteners.erase(
		std::remove(m_vlisteners.begin(), m_vlisteners.end(), _pListener),
		m_vlisteners.end()
	);
}