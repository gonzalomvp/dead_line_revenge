#include "common/stdafx.h"
#include "Component.h"

#include "entities/Entity.h"

void CComponent::init() {
	ASSERT(m_pOwner);

	m_pOwner->addComponent(this);
	if (m_fActivationTimer <= 0.0f) {
		activate();
	}
}

void CComponent::activate() {
	m_bIsActive = true;
}

void CComponent::deactivate() {
	m_bIsActive = false;
}

void CComponent::run(float _fDeltaTime) {
	if (m_fActivationTimer > 0.0f) {
		m_fActivationTimer -= _fDeltaTime;
		if (m_fActivationTimer <= 0.0f) {
			activate();
		}
	}
}
