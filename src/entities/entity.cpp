#include "../common/stdafx.h"
#include "entity.h"

#include "../scenes/world.h"

Entity::~Entity() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		delete *itComponents;
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

void Entity::addComponent(Component* component) {
	m_components.push_back(component);
}

void Entity::receiveMessage(Message* message) {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->receiveMessage(message);
	}
}
