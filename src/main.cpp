#include "./common/stdafx.h"
#include "./scenes/app_manager.h"
#include "./engine/graphics_engine.h"
#include "./input/input_manager.h"
#include "./gui/string_manager.h"

// Global variables
GraphicsEngine* g_graphicsEngine;
InputManager*   g_inputManager;
AppManager* g_appManager;
StringManager*  g_stringManager;
GUIManager*     g_guiManager;
MenuManager*    g_menuManager;
Settings g_settings;

int Main(void) {
	CORE_InitSound();
	g_settings.music = true;
	g_settings.sfx = true;
	g_settings.language = EEnglish;

	GraphicsEngine gfx;
	g_graphicsEngine = &gfx;
	StringManager stringManager;
	g_stringManager = &stringManager;
	g_stringManager->loadLanguage(g_settings.language);
	g_guiManager = GUIManager::instance();
	g_inputManager = new InputManager();
	g_menuManager = new MenuManager();
	g_appManager =  new AppManager();
	g_appManager->switchMode(MODE_MENU);
	
	while (!SYS_GottaQuit()) {
		g_appManager->applyMode();
		g_appManager->processInput();
		g_appManager->run();
		g_appManager->render();

		// Keep system running
		SYS_Pump();
		SYS_Sleep(17);
	}
	CORE_EndSound();
	//delete g_graphicsEngine;
	return 0;
}
