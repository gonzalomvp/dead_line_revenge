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
	checkKeyState(VK_UP);
	checkKeyState(VK_DOWN);
	checkKeyState(VK_RETURN);
	checkKeyState(VK_SPACE);
	checkKeyState(VK_ESCAPE);


	if (SYS_KeyPressed('A')) {
		KeyEvent event;
		event.setType(EKey);
		event.key = 'A';
		for (size_t i = 0; i < m_listeners.size(); ++i)
		{
			m_listeners[i]->onEvent(event);
		}
	}
	if (SYS_KeyPressed('D')) {
		KeyEvent event;
		event.setType(EKey);
		event.key = 'D';
		for (size_t i = 0; i < m_listeners.size(); ++i)
		{
			m_listeners[i]->onEvent(event);
		}
	}
	if (SYS_KeyPressed('W')) {
		KeyEvent event;
		event.setType(EKey);
		event.key = 'W';
		for (size_t i = 0; i < m_listeners.size(); ++i)
		{
			m_listeners[i]->onEvent(event);
		}
	}
	if (SYS_KeyPressed('S')) {
		KeyEvent event;
		event.setType(EKey);
		event.key = 'S';
		for (size_t i = 0; i < m_listeners.size(); ++i)
		{
			m_listeners[i]->onEvent(event);
		}
	}
	if (SYS_KeyPressed(VK_ESCAPE)) {
		KeyEvent event;
		event.setType(EQuit);
		for (size_t i = 0; i < m_listeners.size(); ++i)
		{
			m_listeners[i]->onEvent(event);
		}
	}
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
		for (size_t j = 0; j < m_listeners.size(); ++j)
		{
			m_listeners[j]->onEvent(*m_events[i]);
		}
		//hacer delete tb en el destructor
		delete m_events[i];
	}

	m_events.clear();
}

int InputManager::registerEvent(IListener* listener, TEvent event, int priority) {
	m_listeners.push_back(listener);
	return 0;
}

int InputManager::unregisterEvent(IListener* listener) {
	for (auto itListener = m_listeners.begin(); itListener != m_listeners.end(); ++itListener) {
		if (*itListener == listener) {
			m_listeners.erase(itListener);
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
	if (SYS_KeyPressed(key) &&  m_keys[key] == false ) {
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
	else if (!SYS_KeyPressed(key) && m_keys[key] == true) {
		m_keys[key] = false;
		KeyEvent* event = new KeyEvent();
		event->setType(EKey);
		event->key = key;
		event->action = KeyEvent::KeyReleased;
		g_inputManager->addEvent(event);
	}
}