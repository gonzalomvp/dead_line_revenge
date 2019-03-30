#include "common/stdafx.h"
#include "Component.h"

#include "entities/entity.h"

void Component::init() {
	ASSERT(m_pOwner);

	m_pOwner->addComponent(this);
	if (m_fActivationTimer <= 0.0f) {
		activate();
	}
}

void Component::activate() {
	m_bIsActive = true;
}

void Component::deactivate() {
	m_bIsActive = false;
}

void Component::run(float _fDeltaTime) {
	if (m_fActivationTimer > 0.0f) {
		m_fActivationTimer -= _fDeltaTime;
		if (m_fActivationTimer <= 0.0f) {
			activate();
		}
	}
}
