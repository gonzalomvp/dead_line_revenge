#include "common/stdafx.h"
#include "menu.h"

#include "engine/graphics_engine.h"

CMenu::~CMenu() {
	for (auto itControl = m_vControls.begin(); itControl != m_vControls.end(); ++itControl) {
		DELETE(*itControl);
	}

	if (m_pTitle) {
		DELETE(m_pTitle);
	}
}

void CMenu::activate() {
	CControl::activate();
	for (auto itControl = m_vControls.begin(); itControl != m_vControls.end(); ++itControl) {
		(*itControl)->activate();
	}

	if (m_pTitle) {
		m_pTitle->activate();
	}
}

void CMenu::deactivate() {
	CControl::deactivate();
	for (auto itControl = m_vControls.begin(); itControl != m_vControls.end(); ++itControl) {
		(*itControl)->deactivate();
	}

	if (m_pTitle) {
		m_pTitle->deactivate();
	}
}

void CMenu::run(float _fDeltaTime) {
	if (!m_bIsActive)
		return;

	for (auto itControl = m_vControls.begin(); itControl != m_vControls.end(); ++itControl) {
		(*itControl)->run(_fDeltaTime);
	}
}

void CMenu::setTitle(const std::string& _sTitle) {
	if (m_pTitle) {
		m_pTitle->setText(_sTitle);
	}
	else {
		m_pTitle = NEW(Text, _sTitle, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.8), 1);
		if (!m_bIsActive) {
			m_pTitle->deactivate();
		}
		g_pGraphicsEngine->addGfxEntity(m_pTitle);
	}
	m_pTitle->setPos(vmake((SCR_WIDTH / 2) - (_sTitle.length() / 2.0f * 16), m_pTitle->getPos().y));
}