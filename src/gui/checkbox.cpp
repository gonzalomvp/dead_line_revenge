#include "../common/stdafx.h"
#include "checkbox.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"

//=============================================================================
// Checkbox class
//=============================================================================
Checkbox::~Checkbox() {
	g_graphicsEngine->removeGfxEntity(m_spriteChecked);
	delete m_spriteChecked;

	g_graphicsEngine->removeGfxEntity(m_spriteUnchecked);
	delete m_spriteUnchecked;

	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}


void Checkbox::init(const char* spriteChecked, const char* spriteUnchecked, bool isChecked) {
	m_spriteChecked = new Sprite(g_graphicsEngine->getTexture(spriteChecked), m_pos, m_size, 0.f, 1.f, 1);
	g_graphicsEngine->addGfxEntity(m_spriteChecked);
	m_spriteUnchecked = new Sprite(g_graphicsEngine->getTexture(spriteUnchecked), m_pos, m_size, 0.f, 1.f, 1);
	g_graphicsEngine->addGfxEntity(m_spriteUnchecked);
	m_isChecked = isChecked;
	deactivate();
}

void Checkbox::activate() {
	Control::activate();

	if (m_isChecked) {
		m_spriteChecked->activate();
		m_spriteUnchecked->deactivate();
	}
	else {
		m_spriteUnchecked->activate();
		m_spriteChecked->deactivate();
	}

	m_isPushed = false;

	g_inputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_inputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void Checkbox::deactivate() {
	Control::deactivate();

	m_spriteChecked->deactivate();
	m_spriteUnchecked->deactivate();

	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

bool Checkbox::onEvent(const IInputManager::Event& event) {
	const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);
	//assert(mouseEvent);

	bool isMouseOverButton = mouseEvent.getPos().x >= m_pos.x - m_size.x * 0.5 && mouseEvent.getPos().x <= m_pos.x + m_size.x * 0.5 && mouseEvent.getPos().y >= m_pos.y - m_size.y * 0.5 && mouseEvent.getPos().y <= m_pos.y + m_size.y * 0.5;

	if (mouseEvent.getButton() == SYS_MB_LEFT) {
		switch (mouseEvent.getType()) {
		case IInputManager::EMouseButtonDown:
			if (isMouseOverButton) {
				m_isPushed = true;
			}
			break;
		case IInputManager::EMouseButtonUp:
			if (m_isPushed) {
				m_isPushed = false;
				m_isChecked = !m_isChecked;
				
				if (m_isChecked) {
					m_spriteChecked->activate();
					m_spriteUnchecked->deactivate();
				}
				else {
					m_spriteUnchecked->activate();
					m_spriteChecked->deactivate();
				}

				for (auto itListener = m_listeners.begin(); itListener != m_listeners.end(); ++itListener) {
					(*itListener)->onClick(this);
				}
			}
			
			break;
		case IInputManager::EMouseButtonHold:
			if (!isMouseOverButton) {
				m_isPushed = false;
			}
			break;
		default:
			break;
		}
	}
	return true;
}