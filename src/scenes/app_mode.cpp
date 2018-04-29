#include "../common/stdafx.h"
#include "app_mode.h"

#include "../engine/graphics_engine.h"
#include "../gui/menu.h"
#include "../scenes/world.h"

World* g_world;

//=============================================================================
// AppModeMenu class
//=============================================================================
void AppModeMenu::init() {
	g_menuManager->activateMenu(MenuManager::EMainMenu);
}

AppModeMenu::~AppModeMenu() {
	g_menuManager->deactivateMenu();
}

void AppModeMenu::processInput() const {
	g_inputManager->processInput();
}

void AppModeMenu::run(float deltaTime) const {
	g_menuManager->run();
}

void AppModeMenu::render() const {
	g_graphicsEngine->render();
}

//=============================================================================
// AppModeGame class
//=============================================================================
void AppModeGame::init() {
	g_world = new World(m_level);
	g_world->init();
	g_world->loadLevel();
}

AppModeGame::~AppModeGame() {
	g_menuManager->deactivateMenu();
	delete g_world;
}

void AppModeGame::processInput() const {
	g_inputManager->processInput();
}

void AppModeGame::run(float deltaTime) const {
	g_menuManager->run();
	g_world->run(deltaTime);
}

void AppModeGame::render() const {
	g_graphicsEngine->render();
}