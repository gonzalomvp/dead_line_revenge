#include "../common/stdafx.h"
#include "menu.h"
#include "menu_item.h"
#include "../globals.h"
#include "../scenes/app_manager.h"
#include "../gui/string_manager.h"
#include "../scenes/world.h"
#include "../engine/graphics_engine.h"

Menu::~Menu() {
	g_inputManager->unregisterEvent(this, IInputManager::TEventType::EKeyDown);
}

Menu* Menu::createMainMenu() {
	Menu* menu = new Menu();
	MenuItem* menuItem;
	menuItem = new MenuItem("PLAY_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_PLAY_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("OPTIONS_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_OPTIONS_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("EXIT", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_OPTIONS_EXIT_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menu->m_seletedItem = 0;
	menu->m_menuItems[menu->m_seletedItem]->setFocus(true);
	menu->deactivate();
	return menu;
}

Menu* Menu::createPlayMenu() {
	Menu* menu = new Menu();
	MenuItem* menuItem;
	menuItem = new MenuItem("EASY", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_PLAY_EASY_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("MEDIUM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_PLAY_MEDIUM_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("HARD", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_PLAY_HARD_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("MAIN_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.3), "LTEXT_GUI_BACK_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menu->m_seletedItem = 0;
	menu->m_menuItems[menu->m_seletedItem]->setFocus(true);
	menu->deactivate();
	return menu;
}

Menu* Menu::createOptionsMenu() {
	Menu* menu = new Menu();
	MenuItem* menuItem;
	menuItem = new MenuItem("SETTINGS_MUSIC", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_MUSIC_MENU_ITEM");
	menuItem->init();
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_ON");
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_OFF");
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("SETTINGS_SFX", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_SFX_MENU_ITEM");
	menuItem->init();
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_ON");
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_OFF");
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("SETTINGS_LANGUAGE", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_LANGUAGE_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("MAIN_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.3), "LTEXT_GUI_BACK_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menu->m_seletedItem = 0;
	menu->m_menuItems[menu->m_seletedItem]->setFocus(true);
	menu->deactivate();
	return menu;
}

Menu* Menu::createPauseMenu() {
	Menu* menu = new Menu();
	MenuItem* menuItem;
	menuItem = new MenuItem("RESUME", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_RESUME_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("ABANDON", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_ABANDON_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menu->m_seletedItem = 0;
	menu->m_menuItems[menu->m_seletedItem]->setFocus(true);
	menu->deactivate();
	return menu;
}

Menu* Menu::createGameOverMenu() {
	Menu* menu = new Menu();
	menu->m_title = new Text("GAME OVER", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.8), 1);
	menu->m_title->activate();
	MenuItem* menuItem;
	menuItem = new MenuItem("RETRY", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_RETRY_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("MAIN_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_ABANDON_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menu->m_seletedItem = 0;
	menu->m_menuItems[menu->m_seletedItem]->setFocus(true);
	menu->deactivate();
	return menu;
}

void Menu::run() {
	if (!m_isActive)
		return;

	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->run();
	}
}

void Menu::setSelectedItem(int newOption) {
	if (newOption >= 0 && newOption < m_menuItems.size()) {
		m_menuItems[m_seletedItem]->setFocus(false);
		m_seletedItem = newOption;
		m_menuItems[m_seletedItem]->setFocus(true);
	}
}

void Menu::selectPrevious() {
	m_menuItems[m_seletedItem]->setFocus(false);
	m_seletedItem--;
	if (m_seletedItem < 0)
		m_seletedItem = m_menuItems.size() - 1;
	m_menuItems[m_seletedItem]->setFocus(true);
}

void Menu::selectNext() {
	m_menuItems[m_seletedItem]->setFocus(false);
	m_seletedItem++;
	if (m_seletedItem >= m_menuItems.size())
		m_seletedItem = 0;
	m_menuItems[m_seletedItem]->setFocus(true);
}

void Menu::activate() {
	Control::activate();
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->activate();
	}
	if (m_title) {
		m_title->activate();
	}
	g_inputManager->registerEvent(this, IInputManager::TEventType::EKeyDown);
}

void Menu::deactivate() {
	Control::deactivate();
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->deactivate();
	}
	if (m_title) {
		m_title->deactivate();
	}
	g_inputManager->unregisterEvent(this, IInputManager::TEventType::EKeyDown);
}

bool Menu::onEvent(const IInputManager::Event& event) {
	IInputManager::TEventType eventType = event.getType();

	if (eventType == IInputManager::TEventType::EKeyDown) {
		const IInputManager::KeyEvent keyEvent = *static_cast<const IInputManager::KeyEvent*>(&event);
		switch (keyEvent.getKey()) {
			case VK_UP:
				selectPrevious();
				break;
			case VK_DOWN:
				selectNext();
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

void Menu::setTitle(const char* title) {
	if (m_title) {
		m_title->setText(title);
	}
	else {
		m_title = new Text(title, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.8), 1);
	}
	m_title->setPos(vmake((SCR_WIDTH / 2) - (strlen(title) / 2.0f * 16), m_title->getPos().y));
}

MenuManager::MenuManager() {
	Menu* menu = Menu::createMainMenu();
	menu->addListener(this);
	m_menus[EMainMenu] = menu;

	menu = Menu::createPlayMenu();
	menu->addListener(this);
	m_menus[EPlayMenu] = menu;

	menu = Menu::createOptionsMenu();
	menu->addListener(this);
	m_menus[EOptionsMenu] = menu;

	menu = Menu::createPauseMenu();
	menu->addListener(this);
	m_menus[EPauseMenu] = menu;

	menu = Menu::createGameOverMenu();
	menu->addListener(this);
	m_menus[EGameOverMenu] = menu;
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
	switch (menu)
	{
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
		default:
			break;
	}
	m_activeMenu->activate();
	m_activeMenu->setSelectedItem(0);
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
		g_appManager->switchMode(AppMode::EMENU);
		activateMenu(EMainMenu);
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
		if (g_settings.language == EEnglish)
			g_settings.language = ESpanish;
		else if (g_settings.language == ESpanish)
			g_settings.language = EEnglish;
		g_stringManager->loadLanguage(g_settings.language);
	}
	else if (menuItem->getName() == "RESUME") {
		IInputManager::Event* pauseEvent = new IInputManager::Event(IInputManager::TEventType::EPause);
		pauseEvent->setType(IInputManager::EPause);
		g_inputManager->addEvent(pauseEvent);
	}
	else if (menuItem->getName() == "ABANDON") {
		g_appManager->switchMode(AppMode::EMENU);
	}
	else if (menuItem->getName() == "RETRY") {
		m_activeMenu->deactivate();
		g_world->init();
		g_world->loadLevel();
	}
	else if (menuItem->getName() == "EXIT") {
		exit(0);
	}
}

Menu* MenuManager::getMenu(TMenu menu) {
	return m_menus[menu];
}