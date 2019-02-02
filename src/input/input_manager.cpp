#include "../common/stdafx.h"
#include "input_manager.h"

//=============================================================================
// InputManager class
//=============================================================================
InputManager::~InputManager() {
	for (size_t i = 0; i < m_events.size(); ++i) {
		delete m_events[i];
	}
	m_events.clear();
}

void InputManager::registerEvent(ptr<IListener> listener, TEventType eventType) {
	m_listenersMap[eventType].push_back(listener);
}

void InputManager::unregisterEvent(ptr<IListener> listener, TEventType eventType) {
	for (auto itListener = m_listenersMap[eventType].begin(); itListener != m_listenersMap[eventType].end(); ++itListener) {
		if (*itListener == listener) {
			m_listenersMap[eventType].erase(itListener);
			break;
		}
	}
}

void InputManager::addEvent(Event* event) {
	m_events.push_back(event);
}

void InputManager::processInput() {
	processKeyboard();
	processMouse();

	// Notify to the listeners
	for (size_t i = 0; i < m_events.size(); ++i) {
		for (size_t j = 0; j < m_listenersMap[m_events[i]->getType()].size(); ++j) {
			m_listenersMap[m_events[i]->getType()][j]->onEvent(*m_events[i]);
		}
		delete m_events[i];
	}
	m_events.clear();
}

void InputManager::processKeyboard() {
	checkKeyState('W');
	checkKeyState('A');
	checkKeyState('S');
	checkKeyState('D');
	checkKeyState(VK_UP);
	checkKeyState(VK_DOWN);
	checkKeyState(VK_RETURN);
	checkKeyState(VK_SPACE);
	checkKeyState(VK_ESCAPE);
}

void InputManager::processMouse() {
	ivec2 sysMousePos = SYS_MousePos();
	vec2 mousePos = vmake(static_cast<float>(sysMousePos.x), static_cast<float>(sysMousePos.y));
	checkMouseButtonState(SYS_MB_LEFT, mousePos);
	checkMouseButtonState(SYS_MB_RIGHT, mousePos);
	MouseEvent* mouseEvent = new MouseEvent(EMouseMove, 0, mousePos);
	g_inputManager->addEvent(mouseEvent);
}

void InputManager::checkKeyState(int key) {
	if (SYS_KeyPressed(key)) {
		if (m_inputStates[key] == false) {
			m_inputStates[key] = true;
			KeyEvent* keyEvent = new KeyEvent(TEventType::EKeyDown, key);
			g_inputManager->addEvent(keyEvent);

			if (key == VK_ESCAPE) {
				Event* pauseEvent = new Event(TEventType::EPause);
				g_inputManager->addEvent(pauseEvent);
			}
		}
		KeyEvent* keyEvent = new KeyEvent(TEventType::EKeyHold, key);
		g_inputManager->addEvent(keyEvent);
	}
	else if (!SYS_KeyPressed(key) && m_inputStates[key] == true) {
		m_inputStates[key] = false;
		KeyEvent* keyEvent = new KeyEvent(TEventType::EKeyUp, key);
		g_inputManager->addEvent(keyEvent);
	}
}

void InputManager::checkMouseButtonState(int button, const vec2& mousePos) {
	if (SYS_MouseButonPressed(button)) {
		if (m_inputStates[button] == false) {
			m_inputStates[button] = true;
			MouseEvent* mouseEvent = new MouseEvent(EMouseButtonDown, button, mousePos);
			g_inputManager->addEvent(mouseEvent);
		}
		MouseEvent* mouseEvent = new MouseEvent(EMouseButtonHold, button, mousePos);
		g_inputManager->addEvent(mouseEvent);
	}
	else if (m_inputStates[button] == true) {
		m_inputStates[button] = false;
		MouseEvent* mouseEvent = new MouseEvent(EMouseButtonUp, button, mousePos);
		g_inputManager->addEvent(mouseEvent);
	}
}