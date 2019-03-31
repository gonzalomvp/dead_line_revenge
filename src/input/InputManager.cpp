#include "common/stdafx.h"
#include "InputManager.h"

#include <algorithm>

CInputManager::~CInputManager() {
	for (size_t i = 0; i < m_vEvents.size(); ++i) {
		DELETE(m_vEvents[i]);
	}
}

void CInputManager::registerEvent(IListener* _pListener, EEventType _eEventType) {
	ASSERT(_pListener);
	m_mlisteners[_eEventType].push_back(_pListener);
}

void CInputManager::unregisterEvent(IListener* _pListener, EEventType _eEventType) {
	if (m_mlisteners.count(_eEventType)) {
		m_mlisteners[_eEventType].erase(
			std::remove(m_mlisteners[_eEventType].begin(), m_mlisteners[_eEventType].end(), _pListener),
			m_mlisteners[_eEventType].end()
		);
	}
}

void CInputManager::addEvent(CEvent* _pEvent) {
	ASSERT(_pEvent);
	m_vEvents.push_back(_pEvent);
}

void CInputManager::processInput() {
	processKeyboard();
	processMouse();

	// Notify to the listeners
	for (size_t i = 0; i < m_vEvents.size(); ++i) {
		for (size_t j = 0; j < m_mlisteners[m_vEvents[i]->getType()].size(); ++j) {
			m_mlisteners[m_vEvents[i]->getType()][j]->onEvent(*m_vEvents[i]);
		}
		DELETE(m_vEvents[i]);
	}
	m_vEvents.clear();
}

//=============================================================================
// Keyboard driver
//=============================================================================

void CInputManager::processKeyboard() {
	checkKeyState('W');
	checkKeyState('A');
	checkKeyState('S');
	checkKeyState('D');
	checkKeyState(VK_SPACE);
	checkKeyState(VK_ESCAPE);
	//checkKeyState(VK_UP);
	//checkKeyState(VK_DOWN);
	//checkKeyState(VK_RETURN);
	//checkKeyState(VK_SPACE);
}

void CInputManager::checkKeyState(int _iKey) {
	// Check if the key is pressed
	if (SYS_KeyPressed(_iKey)) {
		// Key down
		if (m_mIputStates[_iKey] == false) {
			m_mIputStates[_iKey] = true;
			CKeyEvent* pKeyEvent = NEW(CKeyEvent, EEventType::EKeyDown, _iKey);
			g_pInputManager->addEvent(pKeyEvent);

			if (_iKey == VK_ESCAPE) {
				CEvent* pPauseEvent = NEW(CEvent, EEventType::EPause);
				g_pInputManager->addEvent(pPauseEvent);
			}
		}
		// Key is being hold
		CKeyEvent* pKeyEvent = NEW(CKeyEvent, EEventType::EKeyHold, _iKey);
		g_pInputManager->addEvent(pKeyEvent);
	}
	// Key up
	else if (!SYS_KeyPressed(_iKey) && m_mIputStates[_iKey] == true) {
		m_mIputStates[_iKey] = false;
		CKeyEvent* pKeyEvent = NEW(CKeyEvent, EEventType::EKeyUp, _iKey);
		g_pInputManager->addEvent(pKeyEvent);
	}
}

//=============================================================================
// Mouse driver
//=============================================================================

void CInputManager::processMouse() {
	ivec2 v2SysMousePos = SYS_MousePos();
	vec2 v2MousePos = vmake(static_cast<float>(v2SysMousePos.x), static_cast<float>(v2SysMousePos.y));
	checkMouseButtonState(SYS_MB_LEFT, v2MousePos);
	checkMouseButtonState(SYS_MB_RIGHT, v2MousePos);

	//Mouse position event
	CMouseEvent* pMouseEvent = NEW(CMouseEvent, EMouseMove, 0, v2MousePos);
	g_pInputManager->addEvent(pMouseEvent);
}

void CInputManager::checkMouseButtonState(int _iButton, const vec2& _v2MousePos) {
	// Check if the button is pressed
	if (SYS_MouseButonPressed(_iButton)) {
		// Button down
		if (m_mIputStates[_iButton] == false) {
			m_mIputStates[_iButton] = true;
			CMouseEvent* pMouseEvent = NEW(CMouseEvent, EMouseButtonDown, _iButton, _v2MousePos);
			g_pInputManager->addEvent(pMouseEvent);
		}
		// Button is being hold
		CMouseEvent* pMouseEvent = NEW(CMouseEvent, EMouseButtonHold, _iButton, _v2MousePos);
		g_pInputManager->addEvent(pMouseEvent);
	}
	// Button up
	else if (m_mIputStates[_iButton] == true) {
		m_mIputStates[_iButton] = false;
		CMouseEvent* pMouseEvent = NEW(CMouseEvent, EMouseButtonUp, _iButton, _v2MousePos);
		g_pInputManager->addEvent(pMouseEvent);
	}
}