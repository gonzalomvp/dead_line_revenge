#include "common/stdafx.h"
#include "Menu.h"

#include "engine/GraphicEngine.h"

CMenu::~CMenu() {
	for (auto itControl = m_vControls.begin(); itControl != m_vControls.end(); ++itControl) {
		DELETE(*itControl);
	}
}

void CMenu::activate() {
	CControl::activate();
	for (auto itControl = m_vControls.begin(); itControl != m_vControls.end(); ++itControl) {
		(*itControl)->activate();
	}
}

void CMenu::deactivate() {
	CControl::deactivate();
	for (auto itControl = m_vControls.begin(); itControl != m_vControls.end(); ++itControl) {
		(*itControl)->deactivate();
	}
}

void CMenu::run(float _fDeltaTime) {
	if (!m_bIsActive)
		return;

	for (auto itControl = m_vControls.begin(); itControl != m_vControls.end(); ++itControl) {
		(*itControl)->run(_fDeltaTime);
	}
}

CControl* CMenu::getControlByName(const std::string& _sName) {
	CControl* pControl = nullptr;
	for (auto itControl = m_vControls.begin(); itControl != m_vControls.end(); ++itControl) {
		if ((*itControl)->getName() == _sName) {
			pControl = *itControl;
			break;
		}
	}
	return pControl;
}
