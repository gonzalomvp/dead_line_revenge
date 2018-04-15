#pragma once

#include "input_manager.h"

#define MOUSE_LBUTTON 0x0001
#define MOUSE_RBUTTON 0x0010
#define MOUSE_MBUTTON 0x0100

class MouseInputManager {
public:
	MouseInputManager() : m_lButtonPressed(false), m_rButtonPressed(false), m_mButtonPressed(false) {}
	bool onEvent(const IInputManager::MouseEvent& mouseEvent);

private:
	bool m_lButtonPressed;
	bool m_rButtonPressed;
	bool m_mButtonPressed;
};