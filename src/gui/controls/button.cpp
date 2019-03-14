#include "common/stdafx.h"
#include "button.h"

#include "engine/graphics_engine.h"
#include "gui/string_manager.h"

CButton::CButton(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const char* _psNormalImage, const char* _psPushImage, const std::string& _sText, bool _bNotifyHold, float _fHoldTime, bool _bIsActive)
: CControl(_sName, _v2Pos, _v2Size, _bIsActive)
, m_psNormalImage(_psNormalImage)
, m_psPushImage(_psPushImage)
, m_sText(_sText)
, m_bNotifyHold(_bNotifyHold)
, m_fHoldTime(_fHoldTime)
, m_pSpriteNormal(nullptr)
, m_pSpritePush(nullptr)
, m_pButtonText(nullptr)
{}

CButton::~CButton() {
	if (g_pGraphicsEngine) {
		g_pGraphicsEngine->removeGfxEntity(m_pSpriteNormal);
		g_pGraphicsEngine->removeGfxEntity(m_pSpritePush);
		g_pGraphicsEngine->removeGfxEntity(m_pButtonText);
	}
	DELETE(m_pSpriteNormal);
	DELETE(m_pSpritePush);
	DELETE(m_pButtonText);

	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
	}
}

void CButton::init() {
	m_pSpriteNormal = NEW(Sprite, g_pGraphicsEngine->getTexture(m_psNormalImage), m_v2Pos, m_v2Size, 0.f, 1.f, 2);
	g_pGraphicsEngine->addGfxEntity(m_pSpriteNormal);
	m_pSpritePush = NEW(Sprite, g_pGraphicsEngine->getTexture(m_psPushImage) , m_v2Pos, m_v2Size, 0.f, 1.f, 2);
	g_pGraphicsEngine->addGfxEntity(m_pSpritePush);
	m_pButtonText = NEW(Text, m_sText, vmake(m_v2Pos.x - (m_sText.length() / 2.0f * 16), m_v2Pos.y - 6), 1);
	g_pGraphicsEngine->addGfxEntity(m_pButtonText);
}

void CButton::activate() {
	CControl::activate();

	m_pSpriteNormal->activate();
	m_pSpritePush->deactivate();
	m_pButtonText->activate();
	m_pButtonText->setPos(vmake(m_v2Pos.x - (m_sText.length() / 2.0f * 16), m_v2Pos.y - 6));

	m_bIsPushed = false;
	m_bIsHold = false;
	m_fHoldTimer = 0.0f;

	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void CButton::deactivate() {
	CControl::deactivate();

	m_pSpriteNormal->deactivate();
	m_pSpritePush->deactivate();
	m_pButtonText->deactivate();

	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

void CButton::run(float _fDeltaTime) {
	if (m_bIsActive) {
		CControl::run(_fDeltaTime);

		std::string textToDraw = g_pStringManager->getText(m_sText);
		m_pButtonText->setText(textToDraw.c_str());
		vec2 currentPos = m_pButtonText->getPos();
		currentPos.x = m_v2Pos.x - (textToDraw.length() / 2.0f * 16);
		m_pButtonText->setPos(currentPos);

		if (m_bIsPushed && m_bNotifyHold) {
			m_fHoldTimer += _fDeltaTime;
			if (m_fHoldTimer >= m_fHoldTime) {
				m_bIsHold = true;
				m_fHoldTimer = 0.0f;
				for (auto itListener = m_vListeners.begin(); itListener != m_vListeners.end(); ++itListener) {
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
					m_bIsPushed = true;
					m_fHoldTimer = 0.0f;
					m_pSpritePush->activate();
					m_pSpriteNormal->deactivate();
					m_pButtonText->setPos(vmake(m_v2Pos.x - (m_sText.length() / 2.0f * 16), m_v2Pos.y - 8));
				}
				break;
			case IInputManager::EMouseButtonUp:
				m_pSpriteNormal->activate();
				m_pSpritePush->deactivate();
				m_pButtonText->setPos(vmake(m_v2Pos.x - (m_sText.length() / 2.0f * 16), m_v2Pos.y - 6));
				if (m_bIsPushed && !m_bIsHold) {
					for (auto itListener = m_vListeners.begin(); itListener != m_vListeners.end(); ++itListener) {
						(*itListener)->onClick(this);
					}
				}
				m_bIsPushed = false;
				m_bIsHold = false;
				break;
			case IInputManager::EMouseButtonHold:
				if (!isMouseOverButton) {
					m_pSpriteNormal->activate();
					m_pSpritePush->deactivate();
					m_pButtonText->setPos(vmake(m_v2Pos.x - (m_sText.length() / 2.0f * 16), m_v2Pos.y - 6));
					m_bIsPushed = false;
					m_bIsHold = false;
				}
				break;
			default:
				break;
		}
	}
	return true;
}