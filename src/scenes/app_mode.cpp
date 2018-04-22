#include "../common/stdafx.h"
#include "app_mode.h"

#include "../engine/graphics_engine.h"
#include "../globals.h"
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
	g_inputManager->registerEvent(this, IInputManager::TEvent::EPause, 0);
	ShowCursor(false);
}

AppModeGame::~AppModeGame() {
	g_inputManager->unregisterEvent(this, IInputManager::TEvent::EPause);
	ShowCursor(true);
	g_menuManager->deactivateMenu();
	delete g_world;
}

void AppModeGame::processInput() const {
	g_inputManager->processInput();
}

void AppModeGame::run(float deltaTime) const {
	g_menuManager->run();
	if (!m_isPaused) {
		g_world->run();
	}
}

void AppModeGame::render() const {
	g_graphicsEngine->render();
}

bool AppModeGame::onEvent(const IInputManager::Event& event) {
	IInputManager::TEvent eventType = event.getType();
	if (eventType == IInputManager::TEvent::EPause) {
		m_isPaused = !m_isPaused;
		ShowCursor(m_isPaused);
		if (m_isPaused) {
			g_world->getPlayer()->deactivate();
			g_menuManager->activateMenu(MenuManager::EPauseMenu);
		}
		else {
			g_world->getPlayer()->activate();
			g_menuManager->deactivateMenu();
		}
	}
	return true;
}