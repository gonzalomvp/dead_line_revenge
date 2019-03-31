#pragma once

class CSceneManager;
class CWorld;
class CEntitiesFactory;
class CGraphicEngine;
class CSoundEngine;
class IInputManager;
class CStringManager;
class CMenuManager;

enum ELanguage {
	EEnglish,
	ESpanish,
};

struct TSettings {
	bool music;
	bool sfx;
	float volume;
	ELanguage language;
};

extern CSceneManager*    g_pSceneManager;
extern CWorld*           g_pWorld;
extern CEntitiesFactory* g_pEntitiesFactory;
extern CGraphicEngine*   g_pGraphicEngine;
extern CSoundEngine*     g_pSoundEngine;
extern IInputManager*    g_pInputManager;
extern CStringManager*   g_pStringManager;
extern CMenuManager*     g_pMenuManager;
extern TSettings         g_settings;