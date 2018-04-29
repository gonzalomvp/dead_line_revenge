#pragma once

enum TFaction {
	EAllied,
	EEnemy,
};

enum TLanguage {
	EEnglish,
	ESpanish,
};

class AppManager;
class World;
class GraphicsEngine;
class SoundEngine;
class IInputManager;
class StringManager;
class GUIManager;
class MenuManager;

struct Settings {
	bool music;
	bool sfx;
	TLanguage language;
};

extern AppManager*     g_appManager;
extern World*          g_world;
extern GraphicsEngine* g_graphicsEngine;
extern SoundEngine*    g_soundEngine;
extern IInputManager*  g_inputManager;
extern StringManager*  g_stringManager;
extern GUIManager*     g_guiManager;
extern MenuManager*    g_menuManager;
extern Settings        g_settings;