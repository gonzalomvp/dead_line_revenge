#include "common/stdafx.h"
#include "label.h"

#include "engine/graphics_engine.h"
#include "engine/text.h"
#include "gui/string_manager.h"

namespace {
	const float s_fTextVerticalOffset = -6.0f;
}

CLabel::CLabel(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const std::string& _sText, bool _bIsActive) 
: CControl(_sName, _v2Pos, _v2Size, _bIsActive)
, m_sText(_sText)
, m_pLabelText(nullptr)
{}

CLabel::~CLabel() {
	if (g_pGraphicsEngine) {
		g_pGraphicsEngine->removeGfxEntity(m_pLabelText);
	}
	DELETE(m_pLabelText);
}

void CLabel::init() {
	CControl::init();

	ASSERT(g_pGraphicsEngine && g_pStringManager);

	m_pLabelText = NEW(CText, m_sText, vmake(m_v2Pos.x - g_pStringManager->calculateTextHalfWidth(m_sText), m_v2Pos.y + s_fTextVerticalOffset), 1);
	g_pGraphicsEngine->addGfxEntity(m_pLabelText);
}

void CLabel::activate() {
	CControl::activate();

	ASSERT(m_pLabelText);
	m_pLabelText->activate();
	ASSERT(g_pStringManager);
	m_pLabelText->setPos(vmake(m_v2Pos.x - g_pStringManager->calculateTextHalfWidth(m_sText), m_v2Pos.y + s_fTextVerticalOffset));
}

void CLabel::deactivate() {
	CControl::deactivate();

	ASSERT(m_pLabelText);
	m_pLabelText->deactivate();
}

void CLabel::run(float _fDeltaTime) {
	if (m_bIsActive) {
		CControl::run(_fDeltaTime);

		ASSERT(m_pLabelText);
		ASSERT(g_pStringManager);
		m_pLabelText->setPos(vmake(m_v2Pos.x - g_pStringManager->calculateTextHalfWidth(m_sText), m_v2Pos.y + s_fTextVerticalOffset));
	}
}
