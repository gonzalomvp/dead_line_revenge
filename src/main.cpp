#include "common/stdafx.h"

#include "engine/GraphicEngine.h"
#include "engine/Sprite.h"
#include "engine/SoundEngine.h"
#include "entities/EntitiesFactory.h"
#include "gui/MenuManager.h"
#include "gui/StringManager.h"
#include "input/InputManager.h"
#include "scenes/SceneManager.h"

#include <ctime>

namespace {
	const std::string s_sConfigFile      = "data/config/config.json";
	const std::string s_sBackgroundImage = "data/images/background.png";
	const std::string s_sMusicFile       = "data/audio/music.wav";
}

// Global variables
CGraphicEngine*   g_pGraphicEngine;
CSoundEngine*     g_pSoundEngine;
IInputManager*    g_pInputManager;
CSceneManager*    g_pSceneManager;
CEntitiesFactory* g_pEntitiesFactory;
CStringManager*   g_pStringManager;
CMenuManager*     g_pMenuManager;
TSettings         g_settings;

int Main(void) {
	g_settings.music = true;
	g_settings.sfx = true;
	g_settings.volume = 0.2f;
	g_settings.language = EEnglish;

	g_pGraphicEngine   = NEW(CGraphicEngine);
	g_pSoundEngine     = NEW(CSoundEngine);
	g_pInputManager    = NEW(CInputManager);
	g_pSceneManager    = NEW(CSceneManager);
	g_pEntitiesFactory = NEW(CEntitiesFactory);
	g_pStringManager   = NEW(CStringManager);
	g_pMenuManager     = NEW(CMenuManager);
	g_pMenuManager->init();

	// Load default language
	g_pStringManager->loadLanguage(g_settings.language);

	// Init Menu Scene
	g_pSceneManager->switchScene(IScene::EMENU);

	// Init Entities Factory
	g_pEntitiesFactory->init(s_sConfigFile.c_str());

	//Set Background
	CSprite background(g_pGraphicEngine->getTexture(s_sBackgroundImage), vmake(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f), vmake(SCR_WIDTH, SCR_HEIGHT), 0.0f, 1.0f, 100);
	g_pGraphicEngine->addGfxEntity(&background);

	//Play Music
	g_pSoundEngine->playMusic(s_sMusicFile.c_str());
	
	clock_t cBeginTime = clock();
	clock_t cEndTime;
	
	// Random seed
	srand(cBeginTime);
	while (!SYS_GottaQuit()) {
		// Prepared for deltaTime but not used yet
		cEndTime = clock();
		float fDeltaTime = static_cast<float>(cEndTime - cBeginTime) / 1000.f;
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
	DELETE(g_pGraphicEngine);
	g_pGraphicEngine = nullptr;
	DELETE(g_pSoundEngine);
	g_pSoundEngine = nullptr;

	DELETE(g_pStringManager);
	g_pStringManager = nullptr;
	
	DUMP_UNFREED;

	return 0;
}
