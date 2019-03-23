#include "common/stdafx.h"
#include "component.h"

#include "entities/entity.h"

void Component::init() {
	m_owner->addComponent(this);
	if (m_activationDelay == 0) {
		activate();
	}
}

void Component::activate() {
	m_isActive = true;
}

void Component::deactivate() {
	m_isActive = false;
}

void Component::run(float deltaTime) {
	if (m_activationTimer < m_activationDelay) {
		++m_activationTimer;
		if (m_activationTimer == m_activationDelay) {
			activate();
		}
	}
}
