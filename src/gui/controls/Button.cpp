#include "common/stdafx.h"
#include "Button.h"

#include "engine/GraphicEngine.h"
#include "engine/Sprite.h"
#include "engine/Text.h"
#include "gui/StringManager.h"

namespace {
	const float s_fNormalTextVerticalOffset = -6.0f;
	const float s_fPushTextVerticalOffset = -8.0f;
}

CButton::CButton(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const char* _psNormalImage, const char* _psPushImage, const std::string& _sText, float _fHoldTime, bool _bIsActive)
: CControl(_sName, _v2Pos, _v2Size, _bIsActive)
, m_psNormalImage(_psNormalImage)
, m_psPushImage(_psPushImage)
, m_sText(_sText)
, m_fHoldTime(_fHoldTime)
, m_fHoldTimer(0.0f)
, m_pSpriteNormal(nullptr)
, m_pSpritePush(nullptr)
, m_pButtonText(nullptr)
, m_bIsPushed(false)
, m_bIsHold(false)
{}

CButton::~CButton() {
	if (g_pGraphicEngine) {
		g_pGraphicEngine->removeGfxEntity(m_pSpriteNormal);
		g_pGraphicEngine->removeGfxEntity(m_pSpritePush);
		g_pGraphicEngine->removeGfxEntity(m_pButtonText);
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
	CControl::init();

	ASSERT(g_pGraphicEngine && g_pStringManager);

	m_pSpriteNormal = NEW(CSprite, g_pGraphicEngine->getTexture(m_psNormalImage), m_v2Pos, m_v2Size, 0.f, 1.f, 2);
	g_pGraphicEngine->addGfxEntity(m_pSpriteNormal);
	m_pSpritePush = NEW(CSprite, g_pGraphicEngine->getTexture(m_psPushImage) , m_v2Pos, m_v2Size, 0.f, 1.f, 2);
	g_pGraphicEngine->addGfxEntity(m_pSpritePush);
	m_pButtonText = NEW(CText, m_sText, vmake(m_v2Pos.x - g_pStringManager->calculateTextHalfWidth(m_sText), m_v2Pos.y + s_fNormalTextVerticalOffset), 1);
	g_pGraphicEngine->addGfxEntity(m_pButtonText);
}

void CButton::activate() {
	CControl::activate();

	ASSERT(m_pSpriteNormal && m_pSpritePush && m_pButtonText);
	m_pSpriteNormal->activate();
	m_pSpritePush->deactivate();
	m_pButtonText->activate();
	ASSERT(g_pStringManager);
	m_pButtonText->setPos(vmake(m_v2Pos.x - g_pStringManager->calculateTextHalfWidth(m_sText), m_v2Pos.y + s_fNormalTextVerticalOffset));

	m_bIsPushed = false;
	m_bIsHold = false;
	m_fHoldTimer = 0.0f;

	ASSERT(g_pInputManager);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void CButton::deactivate() {
	CControl::deactivate();

	ASSERT(m_pSpriteNormal && m_pSpritePush && m_pButtonText);
	m_pSpriteNormal->deactivate();
	m_pSpritePush->deactivate();
	m_pButtonText->deactivate();

	ASSERT(g_pInputManager);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

void CButton::run(float _fDeltaTime) {
	if (m_bIsActive) {
		CControl::run(_fDeltaTime);

		ASSERT(m_pSpriteNormal && m_pSpritePush && m_pButtonText);
		ASSERT(g_pStringManager);
		float fTextVerticalOffset = s_fNormalTextVerticalOffset;
		if (m_bIsPushed) {
			fTextVerticalOffset = s_fPushTextVerticalOffset;
		}
		m_pButtonText->setPos(vmake(m_v2Pos.x - g_pStringManager->calculateTextHalfWidth(m_sText), m_v2Pos.y + fTextVerticalOffset));

		if (m_bIsPushed && m_fHoldTime > 0.0f) {
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

bool CButton::onEvent(const IInputManager::CEvent& _event) {
	ASSERT(m_pSpriteNormal && m_pSpritePush && m_pButtonText);
	bool bConsumed = false;
	const IInputManager::CMouseEvent* pMouseEvent = dynamic_cast<const IInputManager::CMouseEvent*>(&_event);
	if (pMouseEvent) {
		// Check if MouseEvent happened over the button
		bool bIsMouseOverButton = pMouseEvent->getPos().x >= m_v2Pos.x - m_v2Size.x * 0.5 && pMouseEvent->getPos().x <= m_v2Pos.x + m_v2Size.x * 0.5 
			&& pMouseEvent->getPos().y >= m_v2Pos.y - m_v2Size.y * 0.5 && pMouseEvent->getPos().y <= m_v2Pos.y + m_v2Size.y * 0.5;

		if (pMouseEvent->getButton() == SYS_MB_LEFT) {
			bConsumed = true;
			switch (pMouseEvent->getType()) {
				case IInputManager::EMouseButtonDown:
					if (bIsMouseOverButton) {
						m_bIsPushed = true;
						m_fHoldTimer = 0.0f;
						m_pSpritePush->activate();
						m_pSpriteNormal->deactivate();
					}
					break;
				case IInputManager::EMouseButtonUp:
					m_pSpriteNormal->activate();
					m_pSpritePush->deactivate();
					if (m_bIsPushed && !m_bIsHold) {
						for (auto itListener = m_vListeners.begin(); itListener != m_vListeners.end(); ++itListener) {
							(*itListener)->onClick(this);
						}
					}
					m_bIsPushed = false;
					m_bIsHold = false;
					break;
				case IInputManager::EMouseButtonHold:
					if (!bIsMouseOverButton) {
						m_pSpriteNormal->activate();
						m_pSpritePush->deactivate();
						m_bIsPushed = false;
						m_bIsHold = false;
					}
					break;
			default:
				break;
			}
		}
	}

	return bConsumed;
}