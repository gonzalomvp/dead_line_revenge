#include "common/stdafx.h"

#include "engine/graphics_engine.h"
#include "engine/sound_engine.h"
#include "entities/entities_factory.h"
#include "gui/menu.h"
#include "gui/string_manager.h"
#include "input/input_manager.h"
#include "scenes/scene_manager.h"

#include <ctime>

// Global variables
GraphicsEngine*   g_pGraphicsEngine;
SoundEngine*      g_pSoundEngine;
IInputManager*    g_pInputManager;
CSceneManager*    g_pSceneManager;
CEntitiesFactory* g_pEntitiesFactory;
StringManager*    g_pStringManager;
MenuManager*      g_pMenuManager;
Settings          g_settings;

int Main(void) {
	g_settings.music = true;
	g_settings.sfx = true;
	g_settings.volume = 0.2f;
	g_settings.language = EEnglish;

	g_pGraphicsEngine  = NEW(GraphicsEngine);
	g_pSoundEngine     = NEW(SoundEngine);
	g_pInputManager    = NEW(InputManager);
	g_pSceneManager    = NEW(CSceneManager);
	g_pEntitiesFactory = NEW(CEntitiesFactory);
	g_pStringManager   = NEW(StringManager);
	g_pMenuManager     = NEW(MenuManager);
	g_pMenuManager->init();

	// Load default language
	g_pStringManager->loadLanguage(g_settings.language);

	// Init Menu Scene
	g_pSceneManager->switchScene(IScene::EMENU);

	// Init Entities Factory
	g_pEntitiesFactory->init("data/config.json");

	//Set Background
	Sprite background(g_pGraphicsEngine->getTexture("data/background.png"), vmake(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f), vmake(SCR_WIDTH, SCR_HEIGHT), 0.0f, 1.0f, 6);
	g_pGraphicsEngine->addGfxEntity(&background);

	//Play Music
	g_pSoundEngine->playMusic("data/audio/music.wav");
	
	clock_t cBeginTime = clock();
	clock_t cEndTime;
	
	// Random seed
	srand(cBeginTime);
	while (!SYS_GottaQuit()) {
		// Prepared for deltaTime but not used yet
		cEndTime = clock();
		float fDeltaTime = static_cast<float>(cEndTime - cBeginTime);
		cBeginTime = cEndTime;

		g_pSceneManager->applyScene();
		g_pSceneManager->processInput();
		g_pSceneManager->run(fDeltaTime);
		g_pSceneManager->render();

		// Keep system running
		SYS_Pump();
		SYS_Sleep(17);
	}

	// Free memory
	DELETE(g_pInputManager);
	g_pInputManager = nullptr;
	DELETE(g_pEntitiesFactory);
	g_pEntitiesFactory = nullptr;
	DELETE(g_pSceneManager);
	g_pSceneManager = nullptr;
	DELETE(g_pMenuManager);
	g_pMenuManager = nullptr;
	DELETE(g_pGraphicsEngine);
	g_pGraphicsEngine = nullptr;
	DELETE(g_pSoundEngine);
	g_pSoundEngine = nullptr;

	DELETE(g_pStringManager);
	g_pStringManager = nullptr;
	
	DUMP_UNFREED;

	return 0;
}
