#include "../common/stdafx.h"
#include "menu.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"
#include "../scenes/app_manager.h"
#include "../scenes/world.h"
#include "menu_item.h"

//=============================================================================
// Component Menu
//=============================================================================
Menu::~Menu() {
	g_inputManager->unregisterEvent(this, IInputManager::TEventType::EKeyDown);
	for (auto itMenuItems = m_menuItems.begin(); itMenuItems != m_menuItems.end(); ++itMenuItems) {
		delete *itMenuItems;
	}
	if (m_title) {
		delete m_title;
	}
}

void Menu::activate() {
	Control::activate();
	for (auto itMenuItems = m_menuItems.begin(); itMenuItems != m_menuItems.end(); ++itMenuItems) {
		(*itMenuItems)->activate();
	}
	if (m_title) {
		m_title->activate();
	}
	setSelectedItem(0);
	g_inputManager->registerEvent(this, IInputManager::TEventType::EKeyDown);
}

void Menu::deactivate() {
	Control::deactivate();
	for (auto itMenuItems = m_menuItems.begin(); itMenuItems != m_menuItems.end(); ++itMenuItems) {
		(*itMenuItems)->deactivate();
	}
	if (m_title) {
		m_title->deactivate();
	}
	g_inputManager->unregisterEvent(this, IInputManager::TEventType::EKeyDown);
}

void Menu::run() {
	if (!m_isActive)
		return;

	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->run();
	}
}

bool Menu::onEvent(const IInputManager::Event& event) {
	IInputManager::TEventType eventType = event.getType();
	if (eventType == IInputManager::TEventType::EKeyDown) {
		const IInputManager::KeyEvent keyEvent = *static_cast<const IInputManager::KeyEvent*>(&event);
		switch (keyEvent.getKey()) {
			case VK_UP:
				selectPreviousItem();
				break;
			case VK_DOWN:
				selectNextItem();
				break;
			case VK_RETURN:
				m_menuItems[m_seletedItem]->nextOption();
				for (auto itListener = m_listeners.begin(); itListener != m_listeners.end(); ++itListener) {
					(*itListener)->onSelected(m_menuItems[m_seletedItem]);
				}
				break;
		}
	}
	return true;
}

void Menu::setSelectedItem(int newOption) {
	if (newOption >= 0 && newOption < static_cast<int>(m_menuItems.size())) {
		m_menuItems[m_seletedItem]->setFocus(false);
		m_seletedItem = newOption;
		m_menuItems[m_seletedItem]->setFocus(true);
	}
}

void Menu::setTitle(const char* title) {
	if (m_title) {
		m_title->setText(title);
	}
	else {
		m_title = new Text(title, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.8), 1);
		if (!m_isActive) {
			m_title->deactivate();
		}
		g_graphicsEngine->addGfxEntity(m_title);
	}
	m_title->setPos(vmake((SCR_WIDTH / 2) - (strlen(title) / 2.0f * 16), m_title->getPos().y));
}

void Menu::selectPreviousItem() {
	m_menuItems[m_seletedItem]->setFocus(false);
	m_seletedItem--;
	if (m_seletedItem < 0) {
		m_seletedItem = m_menuItems.size() - 1;
	}
	m_menuItems[m_seletedItem]->setFocus(true);
}

void Menu::selectNextItem() {
	m_menuItems[m_seletedItem]->setFocus(false);
	m_seletedItem++;
	if (m_seletedItem >= static_cast<int>(m_menuItems.size())) {
		m_seletedItem = 0;
	}
	m_menuItems[m_seletedItem]->setFocus(true);
}

//=============================================================================
// Component MenuManager
//=============================================================================
MenuManager::~MenuManager() {
	for (auto itMenus = m_menus.begin(); itMenus != m_menus.end(); ++itMenus) {
		delete itMenus->second;
	}
}

void MenuManager::init() {
	createMainMenu();
	createPlayMenu();
	createOptionsMenu();
	createPauseMenu();
	createGameOverMenu();
	for (auto itMenus = m_menus.begin(); itMenus != m_menus.end(); ++itMenus) {
		itMenus->second->addListener(this);
	}
}

void MenuManager::run() {
	if (m_activeMenu) {
		m_activeMenu->run();
	}
}

void MenuManager::activateMenu(TMenu menu) {
	if (m_activeMenu) {
		m_activeMenu->deactivate();
		ShowCursor(false);
	}
	switch (menu) {
		case EMainMenu:
			m_activeMenu = m_menus[EMainMenu];
			break;
		case EOptionsMenu:
			m_activeMenu = m_menus[EOptionsMenu];
			break;
		case EPlayMenu:
			m_activeMenu = m_menus[EPlayMenu];
			break;
		case EPauseMenu:
			m_activeMenu = m_menus[EPauseMenu];
			break;
		case EGameOverMenu:
			m_activeMenu = m_menus[EGameOverMenu];
			break;
	}
	m_activeMenu->activate();
	ShowCursor(true);
}

void MenuManager::deactivateMenu() {
	m_activeMenu->deactivate();
	m_activeMenu = nullptr;
	ShowCursor(false);
}

void MenuManager::onSelected(MenuItem* menuItem) {
	if (menuItem->getName() == "PLAY_MENU") {
		activateMenu(EPlayMenu);
	}
	else if (menuItem->getName() == "OPTIONS_MENU") {
		activateMenu(EOptionsMenu);
	}
	else if (menuItem->getName() == "MAIN_MENU") {
		activateMenu(EMainMenu);
		g_appManager->switchMode(AppMode::EMENU);
	}
	else if (menuItem->getName() == "EASY") {
		g_appManager->switchMode(AppMode::EGAME, 1);
	}
	else if (menuItem->getName() == "MEDIUM") {
		g_appManager->switchMode(AppMode::EGAME, 2);
	}
	else if (menuItem->getName() == "HARD") {
		g_appManager->switchMode(AppMode::EGAME, 3);
	}
	else if (menuItem->getName() == "SETTINGS_MUSIC") {
		g_settings.music = !g_settings.music;
	}
	else if (menuItem->getName() == "SETTINGS_SFX") {
		g_settings.sfx = !g_settings.sfx;
	}
	else if (menuItem->getName() == "SETTINGS_LANGUAGE") {
		if (g_settings.language == EEnglish) {
			g_settings.language = ESpanish;
		}
		else if (g_settings.language == ESpanish) {
			g_settings.language = EEnglish;
		}
		g_stringManager->loadLanguage(g_settings.language);
	}
	else if (menuItem->getName() == "RESUME") {
		IInputManager::Event* pauseEvent = new IInputManager::Event(IInputManager::TEventType::EPause);
		g_inputManager->addEvent(pauseEvent);
	}
	else if (menuItem->getName() == "ABANDON") {
		g_appManager->switchMode(AppMode::EMENU);
	}
	else if (menuItem->getName() == "RETRY") {
		m_activeMenu->deactivate();
		g_world->loadLevel();
	}
	else if (menuItem->getName() == "EXIT") {
		exit(0);
	}
}

Menu* MenuManager::getMenu(TMenu menu) {
	return m_menus[menu];
}

void MenuManager::createMainMenu() {
	Menu* menu = new Menu("MAIN_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	MenuItem* menuItem;
	menuItem = new MenuItem("PLAY_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_PLAY_MENU_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("OPTIONS_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_OPTIONS_MENU_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("EXIT", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_OPTIONS_EXIT_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	m_menus[EMainMenu] = menu;
}

void MenuManager::createPlayMenu() {
	Menu* menu = new Menu("PLAY_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	MenuItem* menuItem;
	menuItem = new MenuItem("EASY", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_PLAY_EASY_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("MEDIUM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_PLAY_MEDIUM_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("HARD", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_PLAY_HARD_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("MAIN_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.3), "LTEXT_GUI_BACK_MENU_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	m_menus[EPlayMenu] = menu;
}

void MenuManager::createOptionsMenu() {
	Menu* menu = new Menu("PLAY_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	MenuItem* menuItem;
	menuItem = new MenuItem("SETTINGS_MUSIC", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_MUSIC_MENU_ITEM");
	menuItem->init();
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_ON");
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_OFF");
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("SETTINGS_SFX", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_SFX_MENU_ITEM");
	menuItem->init();
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_ON");
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_OFF");
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("SETTINGS_LANGUAGE", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_LANGUAGE_MENU_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("MAIN_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.3), "LTEXT_GUI_BACK_MENU_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	m_menus[EOptionsMenu] = menu;
}

void MenuManager::createPauseMenu() {
	Menu* menu = new Menu("PAUSE_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	MenuItem* menuItem;
	menuItem = new MenuItem("RESUME", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_RESUME_MENU_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("ABANDON", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_ABANDON_MENU_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	m_menus[EPauseMenu] = menu;
}

void MenuManager::createGameOverMenu() {
	Menu* menu = new Menu("GAMEOVER_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	menu->setTitle("GAME OVER");
	MenuItem* menuItem;
	menuItem = new MenuItem("RETRY", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_RETRY_MENU_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("MAIN_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_ABANDON_MENU_ITEM");
	menuItem->init();
	menu->addMenuItem(menuItem);
	m_menus[EGameOverMenu] = menu;
}