#include "../common/stdafx.h"
#include "../common/sys.h"
#include "../common/core.h"
#include "../common/font.h"
#include "../globals.h"
#include "input_manager.h"
#include "../scenes/app_manager.h"
#include "../scenes/world.h"
#include "mouse_input_manager.h"

void InputManager::processInput() {
	checkKeyState('W');
	checkKeyState('A');
	checkKeyState('S');
	checkKeyState('D');
	checkKeyState(VK_UP);
	checkKeyState(VK_DOWN);
	checkKeyState(VK_RETURN);
	checkKeyState(VK_SPACE);
	checkKeyState(VK_ESCAPE);

	ivec2 mousePos = SYS_MousePos();
	MouseEvent event;
	event.setType(EMouse);
	event.x = mousePos.x;
	event.y = mousePos.y;
	if (SYS_MouseButonPressed(SYS_MB_LEFT)) {
		event.buttonMask = event.buttonMask | MOUSE_LBUTTON;
	}
	if (SYS_MouseButonPressed(SYS_MB_RIGHT)) {
		event.buttonMask = event.buttonMask | MOUSE_RBUTTON;
	}

	m_mouseInputManager->onEvent(event);

	for (size_t i = 0; i < m_events.size(); ++i)
	{
		for (size_t j = 0; j < m_listenersMap[m_events[i]->getType()].size(); ++j)
		{
			m_listenersMap[m_events[i]->getType()][j]->onEvent(*m_events[i]);
		}
		//hacer delete tb en el destructor
		delete m_events[i];
	}

	m_events.clear();
}

int InputManager::registerEvent(IListener* listener, TEvent event, int priority) {
	//m_listeners.push_back(listener);
	m_listenersMap[event].push_back(listener);
	return 0;
}

int InputManager::unregisterEvent(IListener* listener, TEvent event) {
	for (auto itListener = m_listenersMap[event].begin(); itListener != m_listenersMap[event].end(); ++itListener) {
		if (*itListener == listener) {
			m_listenersMap[event].erase(itListener);
			break;
		}
	}
	return 0;
}

void InputManager::addEvent(Event* event) {
	m_events.push_back(event);
}

InputManager::InputManager() {
	m_mouseInputManager = new MouseInputManager();
}

InputManager::~InputManager() {
	delete m_mouseInputManager;
}

void InputManager::checkKeyState(int key) {
	if (SYS_KeyPressed(key)) {
		if (m_keys[key] == false) {
			m_keys[key] = true;
			KeyEvent* event = new KeyEvent();
			event->setType(EKey);
			event->key = key;
			event->action = KeyEvent::KeyPressed;
			g_inputManager->addEvent(event);

			if (key == VK_ESCAPE) {
				event = new KeyEvent();
				event->setType(EPause);
				g_inputManager->addEvent(event);
			}
		}
		else {
			KeyEvent* event = new KeyEvent();
			event = new KeyEvent();
			event->setType(EKey);
			event->key = key;
			event->action = KeyEvent::KeyDown;
			g_inputManager->addEvent(event);
		}
	}
	else if (!SYS_KeyPressed(key) && m_keys[key] == true) {
		m_keys[key] = false;
		KeyEvent* event = new KeyEvent();
		event->setType(EKey);
		event->key = key;
		event->action = KeyEvent::KeyReleased;
		g_inputManager->addEvent(event);
	}
	
}