#pragma once

class AppManager;
class World;
class GraphicsEngine;
class SoundEngine;
class IInputManager;
class StringManager;
class MenuManager;

enum TLanguage {
	EEnglish,
	ESpanish,
};

struct Settings {
	bool music;
	bool sfx;
	float volume;
	TLanguage language;
};

extern AppManager*     g_appManager;
extern World*          g_world;
extern GraphicsEngine* g_graphicsEngine;
extern SoundEngine*    g_soundEngine;
extern IInputManager*  g_inputManager;
extern StringManager*  g_stringManager;
extern MenuManager*    g_menuManager;
extern Settings            g_settings;