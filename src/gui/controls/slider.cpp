#include "common/stdafx.h"
#include "slider.h"

#include "engine/graphics_engine.h"
#include "gui/string_manager.h"

namespace {
	const float s_fTextVerticalOffset = -6.0f;
}

CSlider::CSlider(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const char* _psLeftNormalImage, const char* _psLeftPushImage, const char* _psRightNormalImage, const char* _psRightPushImage, const char* _psBarImage, const char* _psBallImage, float _fValue, float _fIncrement, bool _bIsActive)
: CControl(_sName, _v2Pos, _v2Size, _bIsActive)
, m_psLeftNormalImage(_psLeftNormalImage)
, m_psLeftPushImage(_psLeftPushImage)
, m_psRightNormalImage(_psRightNormalImage)
, m_psRightPushImage(_psRightPushImage)
, m_psBarImage(_psBarImage)
, m_psBallImage(_psBallImage)
, m_fValue(_fValue)
, m_fIncrement(_fIncrement)
, m_bIsBallPushed(false)
, m_bIsBarPushed(false)
, m_pLeftButton(nullptr)
, m_pRightButton(nullptr)
, m_pSpriteBar(nullptr)
, m_pSpriteBall(nullptr)
, m_pSliderText(nullptr)
{}

CSlider::~CSlider() {
	DELETE(m_pLeftButton);
	DELETE(m_pRightButton);

	if (g_pGraphicsEngine) {
		g_pGraphicsEngine->removeGfxEntity(m_pSpriteBar);
		g_pGraphicsEngine->removeGfxEntity(m_pSpriteBall);
		g_pGraphicsEngine->removeGfxEntity(m_pSliderText);
	}
	DELETE(m_pSpriteBar);
	DELETE(m_pSpriteBall);
	DELETE(m_pSliderText);

	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
	}
}

void CSlider::init() {
	CControl::init();

	ASSERT(g_pGraphicsEngine && g_pStringManager);

	m_pLeftButton = NEW(CButton, m_sName, vmake(m_v2Pos.x - (m_v2Size.x + 32.0f) / 2.0f, m_v2Pos.y), vmake(32.0f, 32.0f), m_psLeftNormalImage, m_psLeftPushImage, "", 0.15f);
	m_pLeftButton->init();
	m_pLeftButton->addListener(this);

	m_pRightButton = NEW(CButton, m_sName, vmake(m_v2Pos.x + (m_v2Size.x + 32.0f) / 2.0f, m_v2Pos.y), vmake(32.0f, 32.0f), m_psRightNormalImage, m_psRightPushImage, "", 0.15f);
	m_pRightButton->init();
	m_pRightButton->addListener(this);

	m_pSpriteBar = NEW(Sprite, g_pGraphicsEngine->getTexture(m_psBarImage), m_v2Pos, vmake(m_v2Size.x, 5.0f), 0.f, 1.f, 2);
	g_pGraphicsEngine->addGfxEntity(m_pSpriteBar);

	m_pSpriteBall = NEW(Sprite, g_pGraphicsEngine->getTexture(m_psBallImage), m_v2Pos, vmake(20.0f, 20.0f), 0.f, 1.f, 1);
	g_pGraphicsEngine->addGfxEntity(m_pSpriteBall);

	m_pSliderText = NEW(Text, "", vmake(m_v2Pos.x + m_v2Size.x * 0.5f + m_pRightButton->getSize().x, m_v2Pos.y + s_fTextVerticalOffset), 1);
	g_pGraphicsEngine->addGfxEntity(m_pSliderText);
}

void CSlider::activate() {
	CControl::activate();

	ASSERT(m_pLeftButton && m_pRightButton && m_pSpriteBar && m_pSpriteBall && m_pSliderText);
	m_pLeftButton->activate();
	m_pRightButton->activate();
	m_pSpriteBar->activate();
	m_pSpriteBall->activate();
	m_pSliderText->activate();

	m_bIsBallPushed = false;
	m_bIsBarPushed = false;
	setValue(m_fValue);

	ASSERT(g_pInputManager);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void CSlider::deactivate() {
	CControl::deactivate();

	ASSERT(m_pLeftButton && m_pRightButton && m_pSpriteBar && m_pSpriteBall && m_pSliderText);
	m_pLeftButton->deactivate();
	m_pRightButton->deactivate();
	m_pSpriteBar->deactivate();
	m_pSpriteBall->deactivate();
	m_pSliderText->deactivate();

	ASSERT(g_pInputManager);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

void CSlider::run(float _fDeltaTime) {
	if (m_bIsActive) {
		CControl::run(_fDeltaTime);

		ASSERT(m_pLeftButton && m_pRightButton);
		m_pLeftButton->run(_fDeltaTime);
		m_pRightButton->run(_fDeltaTime);
	}
}

bool CSlider::onEvent(const IInputManager::CEvent& _event) {
	ASSERT(m_pSpriteBall && m_pSpriteBar);
	bool bConsumed = false;
	const IInputManager::CMouseEvent* pMouseEvent = dynamic_cast<const IInputManager::CMouseEvent*>(&_event);
	if (pMouseEvent) {
		// Check if MouseEvent happened over the slider ball
		bool bIsMouseOverBall = pMouseEvent->getPos().x >= m_pSpriteBall->getPos().x - m_pSpriteBall->getSize().x * 0.5f && pMouseEvent->getPos().x <= m_pSpriteBall->getPos().x + m_pSpriteBall->getSize().x * 0.5f
			&& pMouseEvent->getPos().y >= m_pSpriteBall->getPos().y - m_pSpriteBall->getSize().y * 0.5f && pMouseEvent->getPos().y <= m_pSpriteBall->getPos().y + m_pSpriteBall->getSize().y * 0.5f;
		// Check if MouseEvent happened over the slider bar
		bool bIsMouseOverBar = pMouseEvent->getPos().x >= m_pSpriteBar->getPos().x - m_pSpriteBar->getSize().x * 0.5f && pMouseEvent->getPos().x <= m_pSpriteBar->getPos().x + m_pSpriteBar->getSize().x * 0.5f
			&& pMouseEvent->getPos().y >= m_pSpriteBar->getPos().y - m_pSpriteBar->getSize().y * 2.0f && pMouseEvent->getPos().y <= m_pSpriteBar->getPos().y + m_pSpriteBar->getSize().y * 2.0f;

		if (pMouseEvent->getButton() == SYS_MB_LEFT) {
			bConsumed = true;
			switch (pMouseEvent->getType()) {
				case IInputManager::EMouseButtonDown:
					if (bIsMouseOverBall) {
						m_bIsBallPushed = true;
					}
					else if (bIsMouseOverBar) {
						m_bIsBarPushed = true;
					}
					break;
				case IInputManager::EMouseButtonUp:
					if (m_bIsBarPushed) {
						if (pMouseEvent->getPos().x > m_pSpriteBall->getPos().x) {
							setValue(m_fValue + m_fIncrement);
						}
						else {
							setValue(m_fValue - m_fIncrement);
						}
					}
					m_bIsBallPushed = false;
					m_bIsBarPushed = false;
					break;
				case IInputManager::EMouseButtonHold:
					if (m_bIsBallPushed) {
						float fInitX = m_v2Pos.x - m_v2Size.x / 2.0f + 5.0f;
						float fEndX = m_v2Pos.x + m_v2Size.x / 2.0f - 5.0f;
						float fNewX = clamp(pMouseEvent->getPos().x, fInitX, fEndX);
						float fNewValue = (fNewX - fInitX) / (fEndX - fInitX);
						setValue(fNewValue);
					}
					else if (!bIsMouseOverBar) {
						m_bIsBarPushed = false;
					}
					break;
				default:
					break;
			}
		}

	}

	return bConsumed;
}

void CSlider::onClick(CButton* _pButton) {
	ASSERT(_pButton);
	if (_pButton == m_pRightButton) {
		setValue(m_fValue + m_fIncrement);
	}
	else if (_pButton == m_pLeftButton) {
		setValue(m_fValue - m_fIncrement);
	}
}

void CSlider::setValue(float _fValue) {
	m_fValue = clamp(_fValue, 0.0f, 1.0f);

	ASSERT(m_pSpriteBall);
	if (m_pSpriteBall) {
		float fInitX = m_v2Pos.x - m_v2Size.x / 2.0f + 5.0f;
		float fEndX = m_v2Pos.x + m_v2Size.x / 2.0f - 5.0f;
		float fBarLength = fEndX - fInitX;
		m_pSpriteBall->setPos(vmake(fInitX + (fBarLength)* m_fValue, m_v2Pos.y));
	}
	ASSERT(m_pSliderText);
	if (m_pSliderText) {
		m_pSliderText->setText(std::to_string(static_cast<int>(roundf(m_fValue * 100.0f))) + "%");
	}

	for (size_t i = 0; i < m_vListeners.size(); ++i) {
		m_vListeners[i]->onValueChange(this);
	}
}