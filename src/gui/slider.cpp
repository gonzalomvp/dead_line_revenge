#include "../common/stdafx.h"
#include "slider.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"

//=============================================================================
// Slider class
//=============================================================================
Slider::~Slider() {
	DELETE(m_leftButton);
	DELETE(m_rightButton);

	g_pGraphicsEngine->removeGfxEntity(m_spriteBar);
	DELETE(m_spriteBar);

	g_pGraphicsEngine->removeGfxEntity(m_spriteBall);
	DELETE(m_spriteBall);

	g_pGraphicsEngine->removeGfxEntity(m_sliderText);
	DELETE(m_sliderText);

	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

void Slider::init(const char* spriteLeftNormal, const char* spriteLeftPush, const char* spriteRightNormal, const char* spriteRightPush, const char* spriteBar, const char* spriteBall, float value) {
	m_leftButton = NEW(Button, m_name, vmake(m_pos.x - (m_size.x + 32) / 2, m_pos.y), vmake(32, 32));
	m_leftButton->init("data/ui/Slider_Left_Normal.png", "data/ui/Slider_Left_Push.png", "", true);
	m_leftButton->addListener(this);

	m_rightButton = NEW(Button, m_name, vmake(m_pos.x + (m_size.x + 32) / 2, m_pos.y), vmake(32, 32));
	m_rightButton->init("data/ui/Slider_Right_Normal.png", "data/ui/Slider_Right_Push.png", "", true);
	m_rightButton->addListener(this);

	m_spriteBar = NEW(Sprite, g_pGraphicsEngine->getTexture("data/ui/Slider_bar.png"), m_pos, vmake(m_size.x, 5), 0.f, 1.f, 2);
	g_pGraphicsEngine->addGfxEntity(m_spriteBar);

	m_spriteBall = NEW(Sprite, g_pGraphicsEngine->getTexture("data/ui/Slider_ball.png"), m_pos, vmake(20, 20), 0.f, 1.f, 1);
	g_pGraphicsEngine->addGfxEntity(m_spriteBall);

	m_sliderText = NEW(Text, "", vmake(m_pos.x + m_size.x * 0.5f + m_rightButton->getSize().x, m_pos.y - 6), 1);
	g_pGraphicsEngine->addGfxEntity(m_sliderText);

	m_value = value;
}

void Slider::activate() {
	Control::activate();

	m_leftButton->activate();
	m_rightButton->activate();
	m_spriteBar->activate();
	m_spriteBall->activate();
	m_sliderText->activate();

	m_isBarPushed = false;
	m_isBallPushed = false;
	setValue(m_value);

	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void Slider::deactivate() {
	Control::deactivate();

	m_leftButton->deactivate();
	m_rightButton->deactivate();
	m_spriteBar->deactivate();
	m_spriteBall->deactivate();
	m_sliderText->deactivate();

	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

void Slider::run() {
	if (m_isActive) {
		Control::run();

		m_leftButton->run();
		m_rightButton->run();
	}
}

bool Slider::onEvent(const IInputManager::Event& event) {
	const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);
	//assert(mouseEvent);

	bool isMouseOverBall = mouseEvent.getPos().x >= m_spriteBall->getPos().x - m_spriteBall->getSize().x * 0.5 && mouseEvent.getPos().x <= m_spriteBall->getPos().x + m_spriteBall->getSize().x * 0.5 && mouseEvent.getPos().y >= m_spriteBall->getPos().y - m_spriteBall->getSize().y * 0.5 && mouseEvent.getPos().y <= m_spriteBall->getPos().y + m_spriteBall->getSize().y * 0.5;
	bool isMouseOverBar = mouseEvent.getPos().x >= m_spriteBar->getPos().x - m_spriteBar->getSize().x * 0.5 && mouseEvent.getPos().x <= m_spriteBar->getPos().x + m_spriteBar->getSize().x * 0.5 && mouseEvent.getPos().y >= m_spriteBar->getPos().y - m_spriteBar->getSize().y * 2.f && mouseEvent.getPos().y <= m_spriteBar->getPos().y + m_spriteBar->getSize().y * 2.f;

	if (mouseEvent.getButton() == SYS_MB_LEFT) {
		switch (mouseEvent.getType()) {
		case IInputManager::EMouseButtonDown:
			if (isMouseOverBall) {
				m_isBallPushed = true;
			}
			else if (isMouseOverBar) {
				m_isBarPushed = true;
			}
			break;
		case IInputManager::EMouseButtonUp:
			if (m_isBallPushed) {
				m_isBallPushed = false;
				calculateValueFromPosition();
			}
			else if (m_isBarPushed) {
				m_isBarPushed = false;
				if (mouseEvent.getPos().x > m_spriteBall->getPos().x) {
					setValue(m_value + 0.1f);
				}
				else {
					setValue(m_value - 0.1f);
				}
			}
			break;
		case IInputManager::EMouseButtonHold:
			if (m_isBallPushed) {
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
			else if (!isMouseOverBar) {
				m_isBarPushed = false;
			}
			break;
		default:
			break;
		}
	}
	return true;
}

void Slider::onClick(Button* button) {
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

	if (m_spriteBall) {
		float initX = m_pos.x - m_size.x / 2.0f + 5.0f;
		float endX = m_pos.x + m_size.x / 2.0f - 5.0f;
		float barLength = endX - initX;

		m_spriteBall->setPos(vmake(initX + (barLength)* m_value, m_pos.y));
	}
	if (m_sliderText) {
		m_sliderText->setText(std::to_string(static_cast<int>(roundf(m_value * 100.0f))) + "%");
	}

	for (size_t i = 0; i < m_listeners.size(); ++i) {
		m_listeners[i]->onValueChange(this);
	}
}