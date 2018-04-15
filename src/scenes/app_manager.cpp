#include "app_manager.h"

AppManager::~AppManager() {
	if (m_appMode) {
		m_appMode->deactivate();
		delete m_appMode;
	}
}

void AppManager::processInput() {
	m_appMode->processInput();
}

void AppManager::run() {
	m_appMode->run();
}

void AppManager::render() {
	m_appMode->render();
}

void AppManager::applyMode() {
	ModeId currentMode = MODE_NULL;
	if (m_appMode)
		currentMode = m_appMode->getModeId();

	if (m_wantedMode != currentMode) {
		if (m_appMode) {
			m_appMode->deactivate();
			delete m_appMode;
		}

		switch (m_wantedMode) {
			case MODE_NULL:
				break;
			case MODE_MENU:
				m_appMode = new AppModeMenu();
				break;
			case MODE_GAME:
				m_appMode = new AppModeGame(m_level);
				break;
			default:
				break;
		}
		if (m_appMode)
			m_appMode->init();
	}
}