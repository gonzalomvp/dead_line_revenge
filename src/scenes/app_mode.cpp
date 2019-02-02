#include "../common/stdafx.h"
#include "app_mode.h"

#include "../engine/graphics_engine.h"
#include "../gui/menu.h"
#include "../scenes/world.h"

ptr<World> g_world;

//=============================================================================
// AppModeMenu class
//=============================================================================
void AppModeMenu::init() {
	g_menuManager->activateMenu(MenuManager::EMainMenu);
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
	g_menuManager->deactivateMenu();
	g_world = NEW(World, m_level);
	g_world->init();
	g_world->loadLevel();
}

AppModeGame::~AppModeGame() {
	DELETE(g_world);
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