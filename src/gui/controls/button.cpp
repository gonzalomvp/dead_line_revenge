#include "common/stdafx.h"
#include "button.h"

#include "engine/graphics_engine.h"
#include "gui/string_manager.h"

//=============================================================================
// Button class
//=============================================================================

CButton::~CButton() {
	if (g_pGraphicsEngine) {
		g_pGraphicsEngine->removeGfxEntity(m_spriteNormal);
		g_pGraphicsEngine->removeGfxEntity(m_spritePush);
		g_pGraphicsEngine->removeGfxEntity(m_buttonText);
	}
	DELETE(m_spriteNormal);
	DELETE(m_spritePush);
	DELETE(m_buttonText);

	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
	}
}

void CButton::init(const char* normalImage, const char* pushImage, const std::string& text, bool notifyHold, unsigned int holdTime) {
	m_spriteNormal = NEW(Sprite, g_pGraphicsEngine->getTexture(normalImage), m_v2Pos, m_v2Size, 0.f, 1.f, 2);
	g_pGraphicsEngine->addGfxEntity(m_spriteNormal);
	m_spritePush = NEW(Sprite, g_pGraphicsEngine->getTexture(pushImage) , m_v2Pos, m_v2Size, 0.f, 1.f, 2);
	g_pGraphicsEngine->addGfxEntity(m_spritePush);
	m_text = text;
	m_buttonText = NEW(Text, m_text, vmake(m_v2Pos.x - (m_text.length() / 2.0f * 16), m_v2Pos.y - 6), 1);
	g_pGraphicsEngine->addGfxEntity(m_buttonText);
	m_notifyHold = notifyHold;
	m_holdTime = holdTime;
}

void CButton::activate() {
	CControl::activate();

	m_spriteNormal->activate();
	m_spritePush->deactivate();
	m_buttonText->activate();
	m_buttonText->setPos(vmake(m_v2Pos.x - (m_text.length() / 2.0f * 16), m_v2Pos.y - 6));

	m_isPushed = false;
	m_isHold = false;
	m_holdTimer = 0;

	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void CButton::deactivate() {
	CControl::deactivate();

	m_spriteNormal->deactivate();
	m_spritePush->deactivate();
	m_buttonText->deactivate();

	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

void CButton::run(float _fDeltaTime) {
	if (m_bIsActive) {
		CControl::run(_fDeltaTime);

		std::string textToDraw = g_pStringManager->getText(m_text);
		m_buttonText->setText(textToDraw.c_str());
		vec2 currentPos = m_buttonText->getPos();
		currentPos.x = m_v2Pos.x - (textToDraw.length() / 2.0f * 16);
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

bool CButton::onEvent(const IInputManager::CEvent& event) {
	const IInputManager::CMouseEvent mouseEvent = *static_cast<const IInputManager::CMouseEvent*>(&event);
	//assert(mouseEvent);

	bool isMouseOverButton = mouseEvent.getPos().x >= m_v2Pos.x - m_v2Size.x * 0.5 && mouseEvent.getPos().x <= m_v2Pos.x + m_v2Size.x * 0.5 && mouseEvent.getPos().y >= m_v2Pos.y - m_v2Size.y * 0.5 && mouseEvent.getPos().y <= m_v2Pos.y + m_v2Size.y * 0.5;

	if (mouseEvent.getButton() == SYS_MB_LEFT) {
		switch (mouseEvent.getType()) {
			case IInputManager::EMouseButtonDown:
				if (isMouseOverButton) {
					m_isPushed = true;
					m_holdTimer = 0;
					m_spritePush->activate();
					m_spriteNormal->deactivate();
					m_buttonText->setPos(vmake(m_v2Pos.x - (m_text.length() / 2.0f * 16), m_v2Pos.y - 8));
				}
				break;
			case IInputManager::EMouseButtonUp:
				m_spriteNormal->activate();
				m_spritePush->deactivate();
				m_buttonText->setPos(vmake(m_v2Pos.x - (m_text.length() / 2.0f * 16), m_v2Pos.y - 6));
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
					m_buttonText->setPos(vmake(m_v2Pos.x - (m_text.length() / 2.0f * 16), m_v2Pos.y - 6));
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