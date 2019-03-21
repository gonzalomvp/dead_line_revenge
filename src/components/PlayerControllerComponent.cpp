#include "common/stdafx.h"
#include "PlayerControllerComponent.h"

#include "entities/entity.h"
#include "messages/message.h"

CPlayerControllerComponent::~CPlayerControllerComponent() {
	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EKeyHold);
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EMouseButtonDown);
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EMouseButtonUp);
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EMouseButtonHold);
	}
}

void CPlayerControllerComponent::init() {
	Component::init();

	ASSERT(g_pGraphicsEngine);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EKeyHold);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EMouseButtonUp);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EMouseButtonHold);
}

bool CPlayerControllerComponent::onEvent(const IInputManager::CEvent& _event) {
	bool bConsumed = false;
	if (!m_isActive)
		return bConsumed;

	if (const IInputManager::CKeyEvent* pEvent = dynamic_cast<const IInputManager::CKeyEvent*>(&_event)) {
		if (pEvent->getType() == IInputManager::EEventType::EKeyHold) {
			vec2 direction = vmake(0, 0);
			switch (pEvent->getKey()) {
				case 'A':
					direction = vmake(-1, 0);
					break;
				case 'D':
					direction = vmake(1, 0);
					break;
				case 'W':
					direction = vmake(0, 1);
					break;
				case 'S':
					direction = vmake(0, -1);
					break;
				case VK_SPACE:
					MessageReload msgReload;
					m_owner->receiveMessage(&msgReload);
					break;
			}
			if (vlen2(direction) != 0) {
				bConsumed = true;
				MessageAddMovement msgAddMovement;
				msgAddMovement.dir = direction;
				m_owner->receiveMessage(&msgAddMovement);
			}
		}
	}

	else if (const IInputManager::CMouseEvent* pEvent = dynamic_cast<const IInputManager::CMouseEvent*>(&_event)) {
		IInputManager::EEventType eEventType = pEvent->getType();
		if (eEventType == IInputManager::EEventType::EMouseButtonDown || eEventType == IInputManager::EEventType::EMouseButtonUp) {
			if (pEvent->getButton() == SYS_MB_LEFT) {
				MessageFire messageFire;
				if (eEventType == IInputManager::EEventType::EMouseButtonDown) {
					messageFire.isFiring = true;
				}
				else if (eEventType == IInputManager::EEventType::EMouseButtonUp) {
					messageFire.isFiring = false;
				}
				m_owner->receiveMessage(&messageFire);
			}
			else if (pEvent->getButton() == SYS_MB_RIGHT && eEventType == IInputManager::EEventType::EMouseButtonDown) {
				MessageReload msgReload;
				m_owner->receiveMessage(&msgReload);
			}
		}
	}

	return bConsumed;
}