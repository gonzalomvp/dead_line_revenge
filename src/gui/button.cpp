#include "../common/stdafx.h"
#include "button.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"

//=============================================================================
// Button class
//=============================================================================

Button::~Button() {
	g_graphicsEngine->removeGfxEntity(m_spriteNormal);
	delete m_spriteNormal;

	g_graphicsEngine->removeGfxEntity(m_spritePush);
	delete m_spritePush;

	g_graphicsEngine->removeGfxEntity(m_buttonText);
	delete m_buttonText;

	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

void Button::init(const char* normalImage, const char* pushImage, const std::string& text, bool notifyHold, unsigned int holdTime) {
	m_spriteNormal = new Sprite(g_graphicsEngine->getTexture(normalImage), m_pos, m_size, 0.f, 1.f, 2);
	g_graphicsEngine->addGfxEntity(m_spriteNormal);
	m_spritePush = new Sprite(g_graphicsEngine->getTexture(pushImage) , m_pos, m_size, 0.f, 1.f, 2);
	g_graphicsEngine->addGfxEntity(m_spritePush);
	m_text = text;
	m_buttonText = new Text(m_text, vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6), 1);
	g_graphicsEngine->addGfxEntity(m_buttonText);
	m_notifyHold = notifyHold;
	m_holdTime = holdTime;
}

void Button::activate() {
	Control::activate();

	m_spriteNormal->activate();
	m_spritePush->deactivate();
	m_buttonText->activate();
	m_buttonText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6));

	m_isPushed = false;
	m_isHold = false;
	m_holdTimer = 0;

	g_inputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_inputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void Button::deactivate() {
	Control::deactivate();

	m_spriteNormal->deactivate();
	m_spritePush->deactivate();
	m_buttonText->deactivate();

	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

void Button::run() {
	if (m_isActive) {
		Control::run();

		std::string textToDraw = g_stringManager->getText(m_text);
		m_buttonText->setText(textToDraw.c_str());
		vec2 currentPos = m_buttonText->getPos();
		currentPos.x = m_pos.x - (textToDraw.length() / 2.0f * 16);
		m_buttonText->setPos(currentPos);

		if (m_isPushed && m_notifyHold) {
			++m_holdTimer;
			if (m_holdTimer >= m_holdTime) {
				m_isHold = true;
				m_holdTimer = 0;
				for (auto itListener = m_listeners.begin(); itListener != m_listeners.end(); ++itListener) {
					(*itListener)->onClick(this);
				}
			}
		}
	}
}

bool Button::onEvent(const IInputManager::Event& event) {
	const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);
	//assert(mouseEvent);

	bool isMouseOverButton = mouseEvent.getPos().x >= m_pos.x - m_size.x * 0.5 && mouseEvent.getPos().x <= m_pos.x + m_size.x * 0.5 && mouseEvent.getPos().y >= m_pos.y - m_size.y * 0.5 && mouseEvent.getPos().y <= m_pos.y + m_size.y * 0.5;

	if (mouseEvent.getButton() == SYS_MB_LEFT) {
		switch (mouseEvent.getType()) {
			case IInputManager::EMouseButtonDown:
				if (isMouseOverButton) {
					m_isPushed = true;
					m_holdTimer = 0;
					m_spritePush->activate();
					m_spriteNormal->deactivate();
					m_buttonText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 8));
				}
				break;
			case IInputManager::EMouseButtonUp:
				m_spriteNormal->activate();
				m_spritePush->deactivate();
				m_buttonText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6));
				if (m_isPushed && !m_isHold) {
					for (auto itListener = m_listeners.begin(); itListener != m_listeners.end(); ++itListener) {
						(*itListener)->onClick(this);
					}
				}
				m_isPushed = false;
				m_isHold = false;
				break;
			case IInputManager::EMouseButtonHold:
				if (!isMouseOverButton) {
					m_spriteNormal->activate();
					m_spritePush->deactivate();
					m_buttonText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6));
					m_isPushed = false;
					m_isHold = false;
				}
				break;
			default:
				break;
		}
	}
	return true;
}