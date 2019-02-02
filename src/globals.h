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

extern ptr<AppManager>     g_appManager;
extern ptr<World>          g_world;
extern ptr<GraphicsEngine> g_graphicsEngine;
extern ptr<SoundEngine>    g_soundEngine;
extern ptr<IInputManager>  g_inputManager;
extern ptr<StringManager>  g_stringManager;
extern ptr<MenuManager>    g_menuManager;
extern Settings            g_settings;