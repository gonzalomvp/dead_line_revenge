#include "../common/stdafx.h"
#include "checkbox.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"

//=============================================================================
// Checkbox class
//=============================================================================
Checkbox::Checkbox(const std::string& name, const vec2& pos, const vec2& size, const char* spriteOn, const char* spriteOff, const std::string& text, bool isChecked) : Control(name, pos, size, false) {
	m_spriteOn = new Sprite(g_graphicsEngine->getTexture(spriteOn), pos, size, 0.f, 1.f, 1);
	m_spriteOn->setPos(pos);
	m_spriteOn->setSize(size);
	m_spriteOff = new Sprite(g_graphicsEngine->getTexture(spriteOff), pos, size, 0.f, 1.f, 1);
	m_spriteOff->setPos(pos);
	m_spriteOff->setSize(size);
	g_graphicsEngine->addGfxEntity(m_spriteOn);
	g_graphicsEngine->addGfxEntity(m_spriteOff);
	m_pressed = false;
	m_checked = isChecked;
	deactivate();
	//m_text = text;
	//m_gfxText = new Text(m_text, vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6), 1);
	//g_graphicsEngine->addGfxEntity(m_gfxText);
}

void Checkbox::activate() {
	m_spriteOn->deactivate();
	m_spriteOff->activate();
	//m_gfxText->activate();
	//m_gfxText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6));
	m_pressed = false;

	if (m_checked) {
		m_spriteOn->activate();
		m_spriteOff->deactivate();
	}
	else {
		m_spriteOn->deactivate();
		m_spriteOff->activate();
	}

	g_inputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->registerEvent(this, IInputManager::EMouseButtonUp);
}

void Checkbox::deactivate() {
	m_spriteOn->deactivate();
	m_spriteOff->deactivate();
	//m_gfxText->deactivate();
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
}

void Checkbox::run() {
	//std::string textToDraw = g_stringManager->getText(m_text);
	//m_gfxText->setText(textToDraw.c_str());
	//vec2 currentPos = m_gfxText->getPos();
	//currentPos.x = m_pos.x - (textToDraw.length() / 2.0f * 16);
	//m_gfxText->setPos(currentPos);
}

bool Checkbox::onEvent(const IInputManager::Event& event) {
	const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);

	if (mouseEvent.getButton() == SYS_MB_LEFT) {
		if (mouseEvent.getType() == IInputManager::EMouseButtonDown) {
			if (mouseEvent.getPos().x >= m_pos.x - m_size.x * 0.5 && mouseEvent.getPos().x <= m_pos.x + m_size.x * 0.5 && mouseEvent.getPos().y >= m_pos.y - m_size.y * 0.5 && mouseEvent.getPos().y <= m_pos.y + m_size.y * 0.5) {
				m_pressed = true;
				//m_spriteOff->deactivate();
				//m_spriteOn->activate();
				//m_gfxText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 8));
			}
		}
		else {
			//m_spriteOn->deactivate();
			//m_spriteOff->activate();
			//m_gfxText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6));
			if (m_pressed && mouseEvent.getPos().x >= m_pos.x - m_size.x * 0.5 && mouseEvent.getPos().x <= m_pos.x + m_size.x * 0.5 && mouseEvent.getPos().y >= m_pos.y - m_size.y * 0.5 && mouseEvent.getPos().y <= m_pos.y + m_size.y * 0.5) {
				m_checked = !m_checked;
				if (m_checked) {
					m_spriteOff->deactivate();
					m_spriteOn->activate();
				}
				else {
					m_spriteOn->deactivate();
					m_spriteOff->activate();
				}

				for (auto itListener = m_listeners.begin(); itListener != m_listeners.end(); ++itListener) {
					(*itListener)->onClick(this);
				}
			}
			m_pressed = false;
		}
	}

	//if (eventType == IInputManager::TEvent::EMouse) {
	//	const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);

	//	if (mouseEvent.mouseButton == IInputManager::MouseEvent::BLeft && mouseEvent.mouseButtonAction == IInputManager::MouseEvent::AButtonDown
	//		&& mouseEvent.x >= m_pos.x - m_size.x * 0.5 && mouseEvent.x <= m_pos.x + m_size.x * 0.5 && mouseEvent.y >= m_pos.y - m_size.y * 0.5 && mouseEvent.y <= m_pos.y + m_size.y * 0.5) {
	//		m_pressed = true;
	//		g_graphicsEngine->removeSprite(m_spriteOff);
	//		g_graphicsEngine->addSprite(m_spriteOn);
	//	}

	//	if (mouseEvent.mouseButton == IInputManager::MouseEvent::BLeft && mouseEvent.mouseButtonAction == IInputManager::MouseEvent::AButtonUp) {
	//		if (m_pressed && mouseEvent.x >= m_pos.x - m_size.x * 0.5 && mouseEvent.x <= m_pos.x + m_size.x * 0.5 && mouseEvent.y >= m_pos.y - m_size.y * 0.5 && mouseEvent.y <= m_pos.y + m_size.y * 0.5) {
	//			for (size_t i = 0; i < m_listeners.size(); ++i) {
	//				m_listeners[i]->onClick(this);
	//				//temp
	//				m_listeners.clear();
	//			}
	//		}
	//		m_pressed = false;
	//		g_graphicsEngine->removeSprite(m_spriteOn);
	//		g_graphicsEngine->addSprite(m_spriteOff);
	//	}
	//}

	return true;
}