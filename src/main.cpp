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

	g_graphicsEngine = NEW(GraphicsEngine);
	g_soundEngine    = NEW(SoundEngine);
	g_inputManager   = NEW(InputManager);
	g_appManager     = NEW(AppManager);
	g_stringManager  = NEW(StringManager);
	g_menuManager    = NEW(MenuManager);
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
	DELETE(g_appManager);
	g_appManager = nullptr;
	DELETE(g_menuManager);
	g_menuManager = nullptr;
	DELETE(g_graphicsEngine);
	g_graphicsEngine = nullptr;
	DELETE(g_soundEngine);
	g_soundEngine = nullptr;
	DELETE(g_inputManager);
	g_inputManager = nullptr;
	DELETE(g_stringManager);
	g_stringManager = nullptr;
	
	DUMP_UNFREED;

	return 0;
}
