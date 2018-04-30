#include "./common/stdafx.h"
#include "./scenes/app_manager.h"
#include "./engine/graphics_engine.h"
#include "./engine/sound_engine.h"
#include "./input/input_manager.h"
#include "./gui/string_manager.h"
#include "./gui/menu.h"

#include <ctime>

// Global variables
GraphicsEngine* g_graphicsEngine;
SoundEngine*    g_soundEngine;
IInputManager*  g_inputManager;
AppManager*     g_appManager;
StringManager*  g_stringManager;
MenuManager*    g_menuManager;
Settings        g_settings;

int Main(void) {
	g_graphicsEngine = new GraphicsEngine();
	g_soundEngine    = new SoundEngine();
	g_inputManager   = new InputManager();
	g_appManager     = new AppManager();
	g_stringManager  = new StringManager();
	g_menuManager    = new MenuManager();
	g_menuManager->init();

	g_settings.music = true;
	g_settings.sfx = true;
	g_settings.language = EEnglish;

	// Init Menu Scene
	g_appManager->switchMode(AppMode::EMENU);

	// Load default language
	g_stringManager->loadLanguage(g_settings.language);

	//Set Background
	Sprite background(g_graphicsEngine->getTexture("data/background4.png"), vmake(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f), vmake(SCR_WIDTH, SCR_HEIGHT), 0.0f, 1.0f, 3);
	g_graphicsEngine->addGfxEntity(&background);

	Text* fps = new Text("", vmake(600, 10), 1);
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

		// Prepared for deltaTime but not used yet
		float deltaTime = static_cast<float>(endTime - beginTime);
		float sleepTime = 17 - deltaTime;
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

	// Free memory
	delete g_appManager;
	g_appManager = nullptr;
	delete g_menuManager;
	g_menuManager = nullptr;
	delete g_graphicsEngine;
	g_graphicsEngine = nullptr;
	delete g_soundEngine;
	g_soundEngine = nullptr;
	delete g_inputManager;
	g_inputManager = nullptr;
	delete g_stringManager;
	g_stringManager = nullptr;
	
	return 0;
}
