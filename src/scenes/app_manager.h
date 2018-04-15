#pragma once

#include "app_mode.h"

class AppManager {
public:
	AppManager() : m_appMode(nullptr), m_wantedMode(MODE_NULL), m_level(0) {}
	~AppManager();
	void processInput();
	void run();
	void render();

	void applyMode();
	void switchMode(ModeId wantedMode, int level = 0) { m_wantedMode = wantedMode; m_level = level; }
	
private:
	
	AppMode* m_appMode;
	ModeId   m_wantedMode;
	int      m_level;
};