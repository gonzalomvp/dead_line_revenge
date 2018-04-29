#include "./common/stdafx.h"
#include "./scenes/app_manager.h"
#include "./engine/graphics_engine.h"
#include "./engine/sound_engine.h"
#include "./input/input_manager.h"
#include "./gui/string_manager.h"
#include "./gui/gui.h"
#include "./gui/menu.h"

#include <ctime>

// Global variables
GraphicsEngine* g_graphicsEngine;
SoundEngine*    g_soundEngine;
IInputManager*   g_inputManager;
AppManager* g_appManager;
StringManager*  g_stringManager;
GUIManager*     g_guiManager;
MenuManager*    g_menuManager;
Settings g_settings;

int Main(void) {
	
	g_settings.music = true;
	g_settings.sfx = true;
	g_settings.language = EEnglish;

	GraphicsEngine gfx;
	g_graphicsEngine = &gfx;

	SoundEngine soundEngine;
	g_soundEngine = &soundEngine;

	//Set Background
	Sprite background(g_graphicsEngine->getTexture("data/background4.png"), 3, 1.0f, 0, vmake(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f));
	background.setSize(vmake(SCR_WIDTH, SCR_HEIGHT));
	g_graphicsEngine->addGfxEntity(&background);

	StringManager stringManager;
	g_stringManager = &stringManager;
	g_stringManager->loadLanguage(g_settings.language);
	g_guiManager = GUIManager::instance();
	g_inputManager = new InputManager();
	g_menuManager = new MenuManager();
	g_appManager =  new AppManager();
	g_appManager->switchMode(AppMode::EMENU);

	Text* fps = new Text("", 1, vmake(600, 10));
	g_graphicsEngine->addGfxEntity(fps);
	
	double deltaTime = 0;
	int frameCount = 0;
	/* initialize random seed: */
	clock_t beginTime = clock();
	srand(beginTime);
	clock_t endTime;
	int i = 30;

	while (!SYS_GottaQuit()) {
		--i;
		endTime = clock();
		float deltaTime = endTime - beginTime;
		int sleepTime = 17 - deltaTime;
		beginTime = endTime;
		if (i == 0) {
			i = 30;
			fps->setText(std::to_string((int)(1.0f / (deltaTime / 1000))));
		}

		g_appManager->applyMode();
		g_appManager->processInput();
		g_appManager->run(deltaTime);
		g_appManager->render();

		// Keep system running
		SYS_Pump();
		SYS_Sleep(17);
	}
	
	//delete g_graphicsEngine;
	return 0;
}
