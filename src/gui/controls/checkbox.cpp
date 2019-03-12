#include "common/stdafx.h"
#include "checkbox.h"

#include "engine/graphics_engine.h"
#include "gui/string_manager.h"

//=============================================================================
// Checkbox class
//=============================================================================
Checkbox::~Checkbox() {
	if (g_pGraphicsEngine) {
		g_pGraphicsEngine->removeGfxEntity(m_spriteChecked);
		g_pGraphicsEngine->removeGfxEntity(m_spriteUnchecked);
	}
	DELETE(m_spriteChecked);
	DELETE(m_spriteUnchecked);

	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
	}
}


void Checkbox::init(const char* spriteChecked, const char* spriteUnchecked, bool isChecked) {
	m_spriteChecked = NEW(Sprite, g_pGraphicsEngine->getTexture(spriteChecked), m_v2Pos, m_v2Size, 0.f, 1.f, 1);
	g_pGraphicsEngine->addGfxEntity(m_spriteChecked);
	m_spriteUnchecked = NEW(Sprite, g_pGraphicsEngine->getTexture(spriteUnchecked), m_v2Pos, m_v2Size, 0.f, 1.f, 1);
	g_pGraphicsEngine->addGfxEntity(m_spriteUnchecked);
	m_isChecked = isChecked;
	deactivate();
}

void Checkbox::activate() {
	CControl::activate();

	if (m_isChecked) {
		m_spriteChecked->activate();
		m_spriteUnchecked->deactivate();
	}
	else {
		m_spriteUnchecked->activate();
		m_spriteChecked->deactivate();
	}

	m_isPushed = false;

	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void Checkbox::deactivate() {
	CControl::deactivate();

	m_spriteChecked->deactivate();
	m_spriteUnchecked->deactivate();

	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

bool Checkbox::onEvent(const IInputManager::CEvent& event) {
	const IInputManager::CMouseEvent mouseEvent = *static_cast<const IInputManager::CMouseEvent*>(&event);
	//assert(mouseEvent);

	bool isMouseOverButton = mouseEvent.getPos().x >= m_v2Pos.x - m_v2Size.x * 0.5 && mouseEvent.getPos().x <= m_v2Pos.x + m_v2Size.x * 0.5 && mouseEvent.getPos().y >= m_v2Pos.y - m_v2Size.y * 0.5 && mouseEvent.getPos().y <= m_v2Pos.y + m_v2Size.y * 0.5;

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