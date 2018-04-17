#include "menu.h"
#include "menu_item.h"
#include "../globals.h"
#include "../scenes/app_manager.h"
#include "../gui/string_manager.h"
#include "../scenes/world.h"

Menu::~Menu() {
	g_inputManager->unregisterEvent(this);
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
	g_inputManager->registerEvent(this, IInputManager::TEvent::EQuit, 0);
}

void Menu::deactivate() {
	Control::deactivate();
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->deactivate();
	}
	g_inputManager->unregisterEvent(this);
}

bool Menu::onEvent(const IInputManager::Event& event) {
	IInputManager::TEvent eventType = event.getType();

	if (eventType == IInputManager::TEvent::EKey) {
		const IInputManager::KeyEvent keyEvent = *static_cast<const IInputManager::KeyEvent*>(&event);
		if (keyEvent.action == keyEvent.KeyPressed) {
			switch (keyEvent.key)
			{
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
	}

	return true;
}

MenuManager::MenuManager() {
	m_mainMenu = Menu::createMainMenu();
	m_mainMenu->addListener(this);
	m_playMenu = Menu::createPlayMenu();
	m_playMenu->addListener(this);
	m_optionsMenu = Menu::createOptionsMenu();
	m_optionsMenu->addListener(this);
	m_pauseMenu = Menu::createPauseMenu();
	m_pauseMenu->addListener(this);
}

void MenuManager::run() {
	m_activeMenu->run();
}

void MenuManager::activateMenu(TMenu menu) {
	if (m_activeMenu) {
		m_activeMenu->deactivate();
	}
	switch (menu)
	{
		case MenuManager::EMainMenu:
			m_activeMenu = m_mainMenu;
			break;
		case MenuManager::EOptionsMenu:
			m_activeMenu = m_optionsMenu;
			break;
		case MenuManager::EPlayMenu:
			m_activeMenu = m_playMenu;
			break;
		case MenuManager::EPauseMenu:
			m_activeMenu = m_pauseMenu;
			break;
		default:
			break;
	}
	m_activeMenu->activate();
	m_activeMenu->setSelectedItem(0);

}

void MenuManager::deactivateMenu() {
	m_activeMenu->deactivate();
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
	}
	else if (menuItem->getName() == "EASY") {
		g_appManager->switchMode(MODE_GAME, 1);
	}
	else if (menuItem->getName() == "MEDIUM") {
		g_appManager->switchMode(MODE_GAME, 2);
	}
	else if (menuItem->getName() == "HARD") {
		g_appManager->switchMode(MODE_GAME, 3);
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
		IInputManager::KeyEvent* pauseEvent = new IInputManager::KeyEvent();
		pauseEvent->setType(IInputManager::EPause);
		g_inputManager->addEvent(pauseEvent);
	}
	else if (menuItem->getName() == "ABANDON") {
		g_appManager->switchMode(MODE_MENU);
	}
	else if (menuItem->getName() == "EXIT") {
		exit(0);
	}
}