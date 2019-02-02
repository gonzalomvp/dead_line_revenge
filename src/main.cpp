#include "./common/stdafx.h"
#include "./scenes/app_manager.h"
#include "./engine/graphics_engine.h"
#include "./engine/sound_engine.h"
#include "./input/input_manager.h"
#include "./gui/string_manager.h"
#include "./gui/menu.h"

#include <ctime>

// Global variables
ptr<GraphicsEngine> g_graphicsEngine;
ptr<SoundEngine>    g_soundEngine;
ptr<IInputManager>  g_inputManager;
ptr<AppManager>     g_appManager;
ptr<StringManager>  g_stringManager;
ptr<MenuManager>    g_menuManager;
Settings            g_settings;

int Main(void) {
	g_settings.music = true;
	g_settings.sfx = true;
	g_settings.volume = 0.2f;
	g_settings.language = EEnglish;

	g_graphicsEngine = new GraphicsEngine();
	g_soundEngine    = new SoundEngine();
	g_inputManager   = new InputManager();
	g_appManager     = new AppManager();
	g_stringManager  = new StringManager();
	g_menuManager    = new MenuManager();
	g_menuManager->init();

	// Load default language
	g_stringManager->loadLanguage(g_settings.language);

	// Init Menu Scene
	g_appManager->switchMode(AppMode::EMENU);

	//Set Background
	Sprite background(g_graphicsEngine->getTexture("data/background.png"), vmake(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f), vmake(SCR_WIDTH, SCR_HEIGHT), 0.0f, 1.0f, 6);
	g_graphicsEngine->addGfxEntity(&background);

	//Play Music
	g_soundEngine->playMusic("data/audio/music.wav");
	
	clock_t beginTime = clock();
	clock_t endTime;
	
	// Random seed
	srand(beginTime);
	while (!SYS_GottaQuit()) {
		// Prepared for deltaTime but not used yet
		endTime = clock();
		float deltaTime = static_cast<float>(endTime - beginTime);
		beginTime = endTime;

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
	
	DUMP_UNFREED

	return 0;
}
