#include "../common/stdafx.h"
#include "slider.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"

//=============================================================================
// Slider class
//=============================================================================
Slider::Slider(const std::string& name, const vec2& pos, const vec2& size, const char* spriteOn, const char* spriteOff, const std::string& text, float value) : Control(name, pos, size, false) {
	m_leftButton = new Button(name, vmake(pos.x - (size.x + 32) / 2, pos.y), vmake(32, 32), "data/ui/Slider_Left_Push.png", "data/ui/Slider_Left_Normal.png", "");
	m_rightButton = new Button(name, vmake(pos.x + (size.x + 32) / 2, pos.y), vmake(32, 32), "data/ui/Slider_Right_Push.png", "data/ui/Slider_Right_Normal.png", "");
	m_spriteBar = new Sprite(g_graphicsEngine->getTexture("data/ui/Slider_bar.png"), pos, vmake(size.x, 5), 0.f, 1.f, 2);
	g_graphicsEngine->addGfxEntity(m_spriteBar);
	m_spriteBall = new Sprite(g_graphicsEngine->getTexture("data/ui/Slider_ball.png"), pos, vmake(20, 20), 0.f, 1.f, 1);
	g_graphicsEngine->addGfxEntity(m_spriteBall);
	m_ballPressed = false;
	m_barPressed = false;
	m_value = value;
	//m_text = text;
	m_gfxText = new Text("", vmake(m_pos.x + m_size.x * 0.5f + m_rightButton->getSize().x, m_pos.y - 6), 1);
	g_graphicsEngine->addGfxEntity(m_gfxText);
	m_leftButton->addListener(this);
	m_rightButton->addListener(this);
	deactivate();
	
}

void Slider::activate() {
	m_leftButton->activate();
	m_rightButton->activate();
	m_spriteBar->activate();
	m_spriteBall->activate();
	m_gfxText->activate();
	//m_gfxText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6));
	m_ballPressed = false;
	m_barPressed = false;
	setValue(m_value);
	g_inputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_inputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void Slider::deactivate() {
	m_leftButton->deactivate();
	m_rightButton->deactivate();
	m_spriteBar->deactivate();
	m_spriteBall->deactivate();
	m_gfxText->deactivate();
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_inputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

void Slider::run() {
	//m_gfxText->setText(std::to_string(roundf(m_value)));
	//std::string textToDraw = g_stringManager->getText(m_text);
	//m_gfxText->setText(textToDraw.c_str());
	//vec2 currentPos = m_gfxText->getPos();
	//currentPos.x = m_pos.x - (textToDraw.length() / 2.0f * 16);
	//m_gfxText->setPos(currentPos);
	m_leftButton->run();
	m_rightButton->run();
}

bool Slider::onEvent(const IInputManager::Event& event) {
	const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);

	bool clickOnBall = false;
	bool clickOnBar = false;
	if (mouseEvent.getPos().x >= m_spriteBall->getPos().x - m_spriteBall->getSize().x * 0.5 && mouseEvent.getPos().x <= m_spriteBall->getPos().x + m_spriteBall->getSize().x * 0.5 && mouseEvent.getPos().y >= m_spriteBall->getPos().y - m_spriteBall->getSize().y * 0.5 && mouseEvent.getPos().y <= m_spriteBall->getPos().y + m_spriteBall->getSize().y * 0.5) {
		clickOnBall = true;
	}
	if (mouseEvent.getPos().x >= m_spriteBar->getPos().x - m_spriteBar->getSize().x * 0.5 && mouseEvent.getPos().x <= m_spriteBar->getPos().x + m_spriteBar->getSize().x * 0.5 && mouseEvent.getPos().y >= m_spriteBar->getPos().y - m_spriteBar->getSize().y * 2.f && mouseEvent.getPos().y <= m_spriteBar->getPos().y + m_spriteBar->getSize().y * 2.f) {
		clickOnBar = true;
	}

	if (mouseEvent.getButton() == SYS_MB_LEFT) {
		if (mouseEvent.getType() == IInputManager::EMouseButtonDown && clickOnBall) {
			m_ballPressed = true;
		}
		else if (mouseEvent.getType() == IInputManager::EMouseButtonDown && clickOnBar) {
			m_barPressed = true;
		}
		else if (mouseEvent.getType() == IInputManager::EMouseButtonUp && m_ballPressed) {
			m_ballPressed = false;
			calculateValueFromPosition();
		}
		else if (mouseEvent.getType() == IInputManager::EMouseButtonUp && m_barPressed) {
			m_barPressed = false;
			if (mouseEvent.getPos().x > m_spriteBall->getPos().x) {
				setValue(m_value + 0.1f);
			}
			else {
				setValue(m_value - 0.1f);
			}
		}
	}

	if (mouseEvent.getType() == IInputManager::EMouseButtonHold && m_ballPressed) {
		float initX = m_pos.x - m_size.x / 2.0f + 5.0f;
		float endX = m_pos.x + m_size.x / 2.0f - 5.0f;
		float newX = mouseEvent.getPos().x;
		if (newX < initX) {
			newX = initX;
		}
		else if (newX > endX) {
			newX = endX;
		}
		m_spriteBall->setPos(vmake(newX, m_pos.y));
		calculateValueFromPosition();
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

void Slider::onHold(Button* button) {
	float increment = 0.1f; // hacer variable miembro
	if (button == m_leftButton) {
		increment = -increment;
	}
	setValue(m_value + increment);
}

void Slider::calculateValueFromPosition() {
	float initX = m_pos.x - m_size.x / 2.0f + 5.0f;
	float endX = m_pos.x + m_size.x / 2.0f - 5.0f;
	float barLength = endX - initX;

	float distFromInit = m_spriteBall->getPos().x - initX;
	setValue(distFromInit / barLength);
}

void Slider::setValue(float value) {
	m_value = value;
	if (m_value < 0.f) {
		m_value = 0.f;
	}
	else if (m_value > 1.f) {
		m_value = 1.f;
	}

	if (m_gfxText) {
		m_gfxText->setText(std::to_string(static_cast<int>(roundf(m_value * 100.0f))) + "%");
	}
	if (m_spriteBall) {
		float initX = m_pos.x - m_size.x / 2.0f + 5.0f;
		float endX = m_pos.x + m_size.x / 2.0f - 5.0f;
		float barLength = endX - initX;

		m_spriteBall->setPos(vmake(initX + (barLength)* m_value, m_pos.y));
	}
	for (size_t i = 0; i < m_listeners.size(); ++i) {
		m_listeners[i]->onValueChange(this);
	}
}