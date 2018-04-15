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
class InputManager;
class StringManager;
class GUIManager;

struct Settings {
	bool music;
	bool sfx;
	TLanguage language;
};

extern AppManager*     g_appManager;
extern World*          g_world;
extern GraphicsEngine* g_graphicsEngine;
extern InputManager*   g_inputManager;
extern StringManager*  g_stringManager;
extern GUIManager*     g_guiManager;
extern Settings        g_settings;