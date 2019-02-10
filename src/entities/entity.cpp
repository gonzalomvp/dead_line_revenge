#include "../common/stdafx.h"
#include "entity.h"

#include "components/component.h"
#include "components/LifeComponent.h"
#include "components/MovementComponent.h"

Entity::~Entity() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		DELETE(*itComponents);
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

void Entity::receiveMessage(ptr<Message> message) {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->receiveMessage(message);
	}
}

void Entity::addComponent(ptr<Component> component) {
	m_components.push_back(component);
}

void Entity::loadComponents(ptr<const rapidjson::Value> components) {
	int numComponents = (*components).Size();
	for (int i = 0; i < numComponents; ++i) {
		std::string componentName = (*components)[i]["name"].GetString();

		if (componentName == "LifeComponent") {
			CLifeComponent::loadComponent(this, &(*components)[i]["params"]);
		}
		else if (componentName == "MovementComponent") {
			CMovementComponent::loadComponent(this, &(*components)[i]["params"]);
		}
	}
}