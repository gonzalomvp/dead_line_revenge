#pragma once

#include "app_mode.h"

class AppManager {
public:
	AppManager() : m_appMode(nullptr), m_wantedMode(AppMode::ENULL), m_level(0) {}
	~AppManager();

	void processInput()                const;
	void run         (float deltaTime) const;
	void render      ()                const;
	void applyMode   ();
	void switchMode  (AppMode::TMode wantedMode, uint16_t level = 0);
private:
	ptr<AppMode>   m_appMode;
	AppMode::TMode m_wantedMode;
	uint16_t       m_level;
};