#include "common/stdafx.h"
#include "PlayerControllerComponent.h"

#include "entities/Entity.h"
#include "messages/Message.h"

CPlayerControllerComponent::~CPlayerControllerComponent() {
	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EKeyHold);
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EMouseButtonDown);
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EMouseButtonUp);
	}
}

void CPlayerControllerComponent::init() {
	CComponent::init();

	ASSERT(g_pGraphicEngine);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EKeyHold);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EMouseButtonUp);
}

void CPlayerControllerComponent::run(float _fDeltaTime) {
	CComponent::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	TMessageSetMovementDir msgSetMovementDir;
	msgSetMovementDir.dir = m_v2InputDir;
	m_pOwner->receiveMessage(&msgSetMovementDir);

	m_v2InputDir = vmake(0.0f, 0.0f);
}

bool CPlayerControllerComponent::onEvent(const IInputManager::CEvent& _event) {
	bool bConsumed = false;
	if (!m_bIsActive)
		return bConsumed;

	ASSERT(m_pOwner);

	// Process key inputs
	if (const IInputManager::CKeyEvent* pEvent = dynamic_cast<const IInputManager::CKeyEvent*>(&_event)) {
		if (pEvent->getType() == IInputManager::EEventType::EKeyHold) {
			bConsumed = true;
			switch (pEvent->getKey()) {
				case 'A': {
					m_v2InputDir = vadd(m_v2InputDir, vmake(-1.0f, 0.0f));
					break;
				}
				case 'D': {
					m_v2InputDir = vadd(m_v2InputDir, vmake(1.0f, 0.0f));
					break;
				}
				case 'W': {
					m_v2InputDir = vadd(m_v2InputDir, vmake(0.0f, 1.0f));
					break;
				}
				case 'S': {
					m_v2InputDir = vadd(m_v2InputDir, vmake(0.0f, -1.0f));
					break;
				}	
				case VK_SPACE: {
					TMessageReload msgReload;
					m_pOwner->receiveMessage(&msgReload);
					break;
				}
				default: {
					bConsumed = false;
					break;
				}
			}
		}
	}

	// Process mouse inputs
	else if (const IInputManager::CMouseEvent* pEvent = dynamic_cast<const IInputManager::CMouseEvent*>(&_event)) {
		if (pEvent->getButton() == SYS_MB_LEFT) {
			if (pEvent->getType() == IInputManager::EEventType::EMouseButtonDown) {
				bConsumed = true;
				TMessageFire messageFire;
				messageFire.isFiring = true;
				m_pOwner->receiveMessage(&messageFire);
			}
			else if (pEvent->getType() == IInputManager::EEventType::EMouseButtonUp) {
				bConsumed = true;
				TMessageFire messageFire;
				messageFire.isFiring = false;
				m_pOwner->receiveMessage(&messageFire);
			}
		}
		else if (pEvent->getButton() == SYS_MB_RIGHT) {
			if (pEvent->getType() == IInputManager::EEventType::EMouseButtonDown) {
				bConsumed = true;
				TMessageReload msgReload;
				m_pOwner->receiveMessage(&msgReload);
			}
		}
	}
	return bConsumed;
}