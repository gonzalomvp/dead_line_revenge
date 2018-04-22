#include "app_mode.h"
#include "../globals.h"
#include "../common/stdafx.h"
#include "../common/sys.h"
#include "../common/core.h"
#include "../common/font.h"
#include "../scenes/world.h"
#include "app_manager.h"
#include "../engine/graphics_engine.h"
#include "../input/input_manager.h"
#include "../gui/menu.h"
#include "../gui/menu_item.h"
#include "../gui/string_manager.h"

World* g_world;

//=============================================================================
// AppModeMainMenu class

AppModeMenu::AppModeMenu() {
	//m_mainMenu = Menu::createMainMenu();
	//m_mainMenu->addListener(this);
	//m_playMenu = Menu::createPlayMenu();
	//m_playMenu->addListener(this);
	//m_optionsMenu= Menu::createOptionsMenu();
	//m_optionsMenu->addListener(this);
	//m_activeMenu = m_mainMenu;
	//m_activeMenu->activate();
	g_menuManager->activateMenu(MenuManager::EMainMenu);
}

AppModeMenu::~AppModeMenu() {
	g_menuManager->deactivateMenu();
}

//rehacer
void AppModeMenu::processInput() {
	g_inputManager->processInput();
}

void AppModeMenu::run(float deltaTime) {
	g_menuManager->run();
}

void AppModeMenu::render() {
	g_graphicsEngine->render();
}


//=============================================================================
// AppModeGame class

AppModeGame::AppModeGame(int level) {
	g_world = new World(level);
	g_world->init();
	m_isPaused = false;
	g_inputManager->registerEvent(this, IInputManager::TEvent::EKey, 0);
	g_inputManager->registerEvent(this, IInputManager::TEvent::EPause, 0);
	ShowCursor(false);
}

AppModeGame::~AppModeGame() {
	g_inputManager->unregisterEvent(this, IInputManager::TEvent::EKey);
	g_inputManager->unregisterEvent(this, IInputManager::TEvent::EPause);
	//g_inputManager->clearListeners();
	//g_graphicsEngine->removeAllGfxEntities();
	ShowCursor(true);
	delete g_world;
	g_menuManager->deactivateMenu();
}

void AppModeGame::processInput() {
	g_inputManager->processInput();
}

void AppModeGame::run(float deltaTime) {
	g_menuManager->run();
	if (!m_isPaused) {
		g_world->run();
	}
}

void AppModeGame::render() {
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