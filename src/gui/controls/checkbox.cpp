#include "common/stdafx.h"
#include "checkbox.h"

#include "engine/graphics_engine.h"
#include "engine/sprite.h"
#include "gui/string_manager.h"

CCheckbox::CCheckbox(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const char* _psCheckedImage, const char* _psUncheckedImage, bool _bIsChecked, bool _bIsActive)
: CControl(_sName, _v2Pos, _v2Size, _bIsActive)
, m_psCheckedImage(_psCheckedImage)
, m_psUncheckedImage(_psUncheckedImage)
, m_bIsChecked(_bIsChecked)
, m_pSpriteChecked(nullptr)
, m_pSpriteUnchecked(nullptr)
, m_bIsPushed(false)
{}

CCheckbox::~CCheckbox() {
	if (g_pGraphicsEngine) {
		g_pGraphicsEngine->removeGfxEntity(m_pSpriteChecked);
		g_pGraphicsEngine->removeGfxEntity(m_pSpriteUnchecked);
	}
	DELETE(m_pSpriteChecked);
	DELETE(m_pSpriteUnchecked);

	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
		g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
	}
}


void CCheckbox::init() {
	CControl::init();

	ASSERT(g_pGraphicsEngine);

	m_pSpriteChecked = NEW(CSprite, g_pGraphicsEngine->getTexture(m_psCheckedImage), m_v2Pos, m_v2Size, 0.f, 1.f, 1);
	g_pGraphicsEngine->addGfxEntity(m_pSpriteChecked);
	m_pSpriteUnchecked = NEW(CSprite, g_pGraphicsEngine->getTexture(m_psUncheckedImage), m_v2Pos, m_v2Size, 0.f, 1.f, 1);
	g_pGraphicsEngine->addGfxEntity(m_pSpriteUnchecked);
	deactivate();
}

void CCheckbox::activate() {
	CControl::activate();

	ASSERT(m_pSpriteChecked && m_pSpriteUnchecked);
	if (m_bIsChecked) {
		m_pSpriteChecked->activate();
		m_pSpriteUnchecked->deactivate();
	}
	else {
		m_pSpriteUnchecked->activate();
		m_pSpriteChecked->deactivate();
	}

	m_bIsPushed = false;

	ASSERT(g_pInputManager);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->registerEvent(this, IInputManager::EMouseButtonHold);
}

void CCheckbox::deactivate() {
	CControl::deactivate();

	ASSERT(m_pSpriteChecked && m_pSpriteUnchecked);
	m_pSpriteChecked->deactivate();
	m_pSpriteUnchecked->deactivate();

	ASSERT(g_pInputManager);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonDown);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonUp);
	g_pInputManager->unregisterEvent(this, IInputManager::EMouseButtonHold);
}

bool CCheckbox::onEvent(const IInputManager::CEvent& _event) {
	ASSERT(m_pSpriteChecked && m_pSpriteUnchecked);
	bool bConsumed = false;
	const IInputManager::CMouseEvent* pMouseEvent = dynamic_cast<const IInputManager::CMouseEvent*>(&_event);
	if (pMouseEvent) {
		// Check if MouseEvent happened over the checkbox
		bool bIsMouseOverButton = pMouseEvent->getPos().x >= m_v2Pos.x - m_v2Size.x * 0.5 && pMouseEvent->getPos().x <= m_v2Pos.x + m_v2Size.x * 0.5
			&& pMouseEvent->getPos().y >= m_v2Pos.y - m_v2Size.y * 0.5 && pMouseEvent->getPos().y <= m_v2Pos.y + m_v2Size.y * 0.5;

		if (pMouseEvent->getButton() == SYS_MB_LEFT) {
			bConsumed = true;
			switch (pMouseEvent->getType()) {
				case IInputManager::EMouseButtonDown:
					if (bIsMouseOverButton) {
						m_bIsPushed = true;
					}
					break;
				case IInputManager::EMouseButtonUp:
					if (m_bIsPushed) {
						m_bIsPushed = false;
						m_bIsChecked = !m_bIsChecked;

						if (m_bIsChecked) {
							m_pSpriteChecked->activate();
							m_pSpriteUnchecked->deactivate();
						}
						else {
							m_pSpriteUnchecked->activate();
							m_pSpriteChecked->deactivate();
						}

						for (auto itListener = m_listeners.begin(); itListener != m_listeners.end(); ++itListener) {
							(*itListener)->onClick(this);
						}
					}
					break;
				case IInputManager::EMouseButtonHold:
					if (!bIsMouseOverButton) {
						m_bIsPushed = false;
					}
					break;
				default:
					break;
			}
		}
	}

	return bConsumed;
}