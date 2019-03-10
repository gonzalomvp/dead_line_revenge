#pragma once

class CSceneManager;
class CWorld;
class CEntitiesFactory;
class GraphicsEngine;
class SoundEngine;
class IInputManager;
class StringManager;
class CMenuManager;

enum ELanguage {
	EEnglish,
	ESpanish,
};

struct Settings {
	bool music;
	bool sfx;
	float volume;
	ELanguage language;
};

extern CSceneManager*    g_pSceneManager;
extern CWorld*           g_pWorld;
extern CEntitiesFactory* g_pEntitiesFactory;
extern GraphicsEngine*   g_pGraphicsEngine;
extern SoundEngine*      g_pSoundEngine;
extern IInputManager*    g_pInputManager;
extern StringManager*    g_pStringManager;
extern CMenuManager*     g_pMenuManager;
extern Settings          g_settings;