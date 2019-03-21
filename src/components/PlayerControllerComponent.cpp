#include "common/stdafx.h"
#include "PlayerControllerComponent.h"

#include "entities/entity.h"
#include "messages/message.h"

CPlayerControllerComponent::~CPlayerControllerComponent() {
	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EKeyHold);
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EMouseButtonDown);
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EMouseButtonUp);
	}
}

void CPlayerControllerComponent::init() {
	Component::init();

	ASSERT(g_pGraphicsEngine);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EKeyHold);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EMouseButtonUp);
}

bool CPlayerControllerComponent::onEvent(const IInputManager::CEvent& _event) {
	bool bConsumed = false;
	if (!m_isActive)
		return bConsumed;

	ASSERT(m_owner);

	// Process key inputs
	if (const IInputManager::CKeyEvent* pEvent = dynamic_cast<const IInputManager::CKeyEvent*>(&_event)) {
		if (pEvent->getType() == IInputManager::EEventType::EKeyHold) {
			vec2 v2MoveDirection = vmake(0.0f, 0.0f);
			bConsumed = true;
			switch (pEvent->getKey()) {
				case 'A': {
					v2MoveDirection = vmake(-1.0f, 0.0f);
					break;
				}
				case 'D': {
					v2MoveDirection = vmake(1.0f, 0.0f);
					break;
				}
				case 'W': {
					v2MoveDirection = vmake(0.0f, 1.0f);
					break;
				}
				case 'S': {
					v2MoveDirection = vmake(0.0f, -1.0f);
					break;
				}	
				case VK_SPACE: {
					MessageReload msgReload;
					m_owner->receiveMessage(&msgReload);
					break;
				}
				default: {
					bConsumed = false;
					break;
				}
			}
			if (vlen2(v2MoveDirection) != 0) {
				MessageAddMovement msgAddMovement;
				msgAddMovement.dir = v2MoveDirection;
				m_owner->receiveMessage(&msgAddMovement);
			}
		}
	}

	// Process mouse inputs
	else if (const IInputManager::CMouseEvent* pEvent = dynamic_cast<const IInputManager::CMouseEvent*>(&_event)) {
		if (pEvent->getButton() == SYS_MB_LEFT) {
			if (pEvent->getType() == IInputManager::EEventType::EMouseButtonDown) {
				bConsumed = true;
				MessageFire messageFire;
				messageFire.isFiring = true;
				m_owner->receiveMessage(&messageFire);
			}
			else if (pEvent->getType() == IInputManager::EEventType::EMouseButtonUp) {
				bConsumed = true;
				MessageFire messageFire;
				messageFire.isFiring = false;
				m_owner->receiveMessage(&messageFire);
			}
		}
		else if (pEvent->getButton() == SYS_MB_RIGHT) {
			if (pEvent->getType() == IInputManager::EEventType::EMouseButtonDown) {
				bConsumed = true;
				MessageReload msgReload;
				m_owner->receiveMessage(&msgReload);
			}
		}
	}
	return bConsumed;
}