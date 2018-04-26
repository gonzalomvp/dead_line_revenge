#include "../common/stdafx.h"
#include "../common/sys.h"
#include "../common/core.h"
#include "../common/font.h"
#include "../globals.h"


#include "mouse_input_manager.h"

bool MouseInputManager::onEvent(const IInputManager::MouseEvent& mouseEvent) {
	if (mouseEvent.buttonMask & MOUSE_LBUTTON && !m_lButtonPressed) {
		m_lButtonPressed = true;
		IInputManager::MouseEvent* eventToAdd = new IInputManager::MouseEvent();
		eventToAdd->x = mouseEvent.x;
		eventToAdd->y = mouseEvent.y;
		eventToAdd->mouseButton = IInputManager::MouseEvent::BLeft;
		eventToAdd->mouseButtonAction = IInputManager::MouseEvent::AButtonDown;
		g_inputManager->addEvent(eventToAdd);
	}
	if (!(mouseEvent.buttonMask & MOUSE_LBUTTON) && m_lButtonPressed) {
		m_lButtonPressed = false;
		IInputManager::MouseEvent* eventToAdd = new IInputManager::MouseEvent();
		eventToAdd->x = mouseEvent.x;
		eventToAdd->y = mouseEvent.y;
		eventToAdd->mouseButton = IInputManager::MouseEvent::BLeft;
		eventToAdd->mouseButtonAction = IInputManager::MouseEvent::AButtonUp;
		g_inputManager->addEvent(eventToAdd);
	}
	if (mouseEvent.buttonMask & MOUSE_RBUTTON && !m_rButtonPressed)
	{
		m_rButtonPressed = true;
		IInputManager::MouseEvent* eventToAdd = new IInputManager::MouseEvent();
		eventToAdd->x = mouseEvent.x;
		eventToAdd->y = mouseEvent.y;
		eventToAdd->mouseButton = IInputManager::MouseEvent::BRight;
		eventToAdd->mouseButtonAction = IInputManager::MouseEvent::AButtonDown;
		g_inputManager->addEvent(eventToAdd);
	}
	if (!(mouseEvent.buttonMask & MOUSE_RBUTTON) && m_rButtonPressed)
	{
		m_rButtonPressed = false;
		IInputManager::MouseEvent* eventToAdd = new IInputManager::MouseEvent();
		eventToAdd->x = mouseEvent.x;
		eventToAdd->y = mouseEvent.y;
		eventToAdd->mouseButton = IInputManager::MouseEvent::BRight;
		eventToAdd->mouseButtonAction = IInputManager::MouseEvent::AButtonUp;
		g_inputManager->addEvent(eventToAdd);
	}

	IInputManager::MouseEvent* eventToAdd = new IInputManager::MouseEvent();
	eventToAdd->x = mouseEvent.x;
	eventToAdd->y = mouseEvent.y;
	eventToAdd->mouseButtonAction = IInputManager::MouseEvent::AMove;
	g_inputManager->addEvent(eventToAdd);

	return true;
}