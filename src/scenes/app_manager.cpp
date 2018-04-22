#include "app_manager.h"

AppManager::~AppManager() {
	if (m_appMode) {
		delete m_appMode;
	}
}

void AppManager::processInput() const {
	m_appMode->processInput();
}

void AppManager::run(float deltaTime) const {
	m_appMode->run(deltaTime);
}

void AppManager::render() const {
	m_appMode->render();
}

void AppManager::applyMode() {
	AppMode::TMode currentMode = AppMode::ENULL;
	if (m_appMode) {
		currentMode = m_appMode->getModeId();
	}
	if (m_wantedMode != currentMode) {
		if (m_appMode) {
			delete m_appMode;
		}
		switch (m_wantedMode) {
			case AppMode::EMENU:
				m_appMode = new AppModeMenu();
				m_appMode->init();
				break;
			case AppMode::EGAME:
				m_appMode = new AppModeGame(m_level);
				m_appMode->init();
				break;
			case AppMode::ENULL:
				break;
		}	
	}
}

void AppManager::switchMode(AppMode::TMode wantedMode, uint16_t level) { 
	m_wantedMode = wantedMode;
	m_level      = level;
}