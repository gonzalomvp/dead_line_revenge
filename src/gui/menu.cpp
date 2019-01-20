#include "../common/stdafx.h"
#include "menu.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"
#include "../scenes/app_manager.h"
#include "../scenes/world.h"


//=============================================================================
// MenuItem class
//=============================================================================
MenuItem::~MenuItem() {
	delete m_gfxText;
}

void MenuItem::init() {
	m_gfxText = new Text(m_text, m_pos, 1);
	m_gfxText->deactivate();
	g_graphicsEngine->addGfxEntity(m_gfxText);
}

void MenuItem::activate() {
	m_gfxText->activate();
}

void MenuItem::deactivate() {
	m_gfxText->deactivate();
}

void MenuItem::run() {
	std::string textToDraw = g_stringManager->getText(m_text);
	if (m_options.size() > 0) {
		textToDraw += " " + g_stringManager->getText(m_options[m_selectedOption]);
	}
	if (m_hasFocus) {
		textToDraw = "- " + textToDraw + " -";
	}
	m_gfxText->setPos(vmake(m_pos.x - (textToDraw.length() / 2.0f * 16), m_pos.y));
	m_gfxText->setText(textToDraw.c_str());
}

void MenuItem::nextOption() {
	if (m_options.size() > 0) {
		++m_selectedOption;
		if (m_selectedOption >= m_options.size())
			m_selectedOption = 0;
	}
}

//=============================================================================
// Menu class
//=============================================================================
Menu::~Menu() {
	if (g_inputManager) {
		g_inputManager->unregisterEvent(this, IInputManager::TEventType::EKeyDown);
	}
	for (auto itMenuItems = m_menuItems.begin(); itMenuItems != m_menuItems.end(); ++itMenuItems) {
		delete *itMenuItems;
	}
	for (auto itControls = m_controls.begin(); itControls != m_controls.end(); ++itControls) {
		delete *itControls;
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
	for (auto itControls = m_controls.begin(); itControls != m_controls.end(); ++itControls) {
		(*itControls)->activate();
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
	for (auto itControls = m_controls.begin(); itControls != m_controls.end(); ++itControls) {
		(*itControls)->deactivate();
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
	for (auto itControls = m_controls.begin(); itControls != m_controls.end(); ++itControls) {
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

void Menu::setTitle(const std::string& title) {
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
	m_title->setPos(vmake((SCR_WIDTH / 2) - (title.length() / 2.0f * 16), m_title->getPos().y));
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
// MenuManager class
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
	m_activeMenu = m_menus[menu];
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
		g_menuManager->deactivateMenu();
		g_world->loadLevel();
	}
	else if (menuItem->getName() == "EXIT") {
		exit(0);
	}
}

void MenuManager::onClick(Button* button) {
	if (button->getName() == "PLAY_MENU") {
		activateMenu(EPlayMenu);
	}
	else if (button->getName() == "OPTIONS_MENU") {
		activateMenu(EOptionsMenu);
	}
	else if (button->getName() == "MAIN_MENU") {
		activateMenu(EMainMenu);
		g_appManager->switchMode(AppMode::EMENU);
	}
	else if (button->getName() == "EASY") {
		g_appManager->switchMode(AppMode::EGAME, 1);
	}
	else if (button->getName() == "MEDIUM") {
		g_appManager->switchMode(AppMode::EGAME, 2);
	}
	else if (button->getName() == "HARD") {
		g_appManager->switchMode(AppMode::EGAME, 3);
	}
	else if (button->getName() == "SETTINGS_MUSIC") {
		g_settings.music = !g_settings.music;
	}
	else if (button->getName() == "SETTINGS_SFX") {
		g_settings.sfx = !g_settings.sfx;
	}
	else if (button->getName() == "SETTINGS_LANGUAGE") {
		if (g_settings.language == EEnglish) {
			g_settings.language = ESpanish;
		}
		else if (g_settings.language == ESpanish) {
			g_settings.language = EEnglish;
		}
		g_stringManager->loadLanguage(g_settings.language);
	}
	else if (button->getName() == "RESUME") {
		IInputManager::Event* pauseEvent = new IInputManager::Event(IInputManager::TEventType::EPause);
		g_inputManager->addEvent(pauseEvent);
	}
	else if (button->getName() == "ABANDON") {
		g_appManager->switchMode(AppMode::EMENU);
	}
	else if (button->getName() == "RETRY") {
		g_menuManager->deactivateMenu();
		g_world->loadLevel();
	}
	else if (button->getName() == "EXIT") {
		exit(0);
	}
}

Menu* MenuManager::getMenu(TMenu menu) {
	return m_menus[menu];
}

void MenuManager::createMainMenu() {
	Menu* menu = new Menu("MAIN_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	//MenuItem* menuItem;
	//menuItem = new MenuItem("PLAY_MENU", "LTEXT_GUI_PLAY_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6));
	//menuItem->init();
	//menu->addMenuItem(menuItem);
	//menuItem = new MenuItem("OPTIONS_MENU", "LTEXT_GUI_OPTIONS_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5));
	//menuItem->init();
	//menu->addMenuItem(menuItem);
	//menuItem = new MenuItem("EXIT", "LTEXT_GUI_OPTIONS_EXIT_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4));
	//menuItem->init();
	//menu->addMenuItem(menuItem);
	m_menus[EMainMenu] = menu;

	Button* button = new Button("PLAY_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), vmake(150, 35), "data/ui/buttonOn.png", "data/ui/buttonOff.png", "LTEXT_GUI_PLAY_MENU_ITEM");
	button->addListener(this);
	menu->addControl(button);
	button = new Button("OPTIONS_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), vmake(150, 35), "data/ui/buttonOn.png", "data/ui/buttonOff.png", "LTEXT_GUI_OPTIONS_MENU_ITEM");
	button->addListener(this);
	menu->addControl(button);
	button = new Button("EXIT", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), vmake(150, 35), "data/ui/buttonOn.png", "data/ui/buttonOff.png", "LTEXT_GUI_OPTIONS_EXIT_ITEM");
	button->addListener(this);
	menu->addControl(button);



	Checkbox* checkbox = new Checkbox("PLAY_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.3), vmake(35, 35), "data/ui/CheckBox_enabled.png", "data/ui/CheckBox_disabled.png", "LTEXT_GUI_PLAY_MENU_ITEM");
	menu->addControl(checkbox);

	Slider* slider = new Slider("PLAY_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.2), vmake(100, 32), "data/ui/CheckBox_enabled.png", "data/ui/CheckBox_disabled.png", "LTEXT_GUI_PLAY_MENU_ITEM");
	menu->addControl(slider);
}

void MenuManager::createPlayMenu() {
	Menu* menu = new Menu("PLAY_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	MenuItem* menuItem;
	menuItem = new MenuItem("EASY", "LTEXT_GUI_PLAY_EASY_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.65));
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("MEDIUM", "LTEXT_GUI_PLAY_MEDIUM_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.55));
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("HARD", "LTEXT_GUI_PLAY_HARD_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.45));
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("MAIN_MENU", "LTEXT_GUI_BACK_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.35));
	menuItem->init();
	menu->addMenuItem(menuItem);
	m_menus[EPlayMenu] = menu;
}

void MenuManager::createOptionsMenu() {
	Menu* menu = new Menu("PLAY_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	MenuItem* menuItem;
	menuItem = new MenuItem("SETTINGS_MUSIC", "LTEXT_GUI_MUSIC_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.65));
	menuItem->init();
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_ON");
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_OFF");
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("SETTINGS_SFX", "LTEXT_GUI_SFX_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.55));
	menuItem->init();
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_ON");
	menuItem->addOption("LTEXT_GUI_MENU_ITEM_OFF");
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("SETTINGS_LANGUAGE", "LTEXT_GUI_LANGUAGE_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.45));
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("MAIN_MENU", "LTEXT_GUI_BACK_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.35));
	menuItem->init();
	menu->addMenuItem(menuItem);
	m_menus[EOptionsMenu] = menu;
}

void MenuManager::createPauseMenu() {
	Menu* menu = new Menu("PAUSE_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	MenuItem* menuItem;
	menuItem = new MenuItem("RESUME", "LTEXT_GUI_RESUME_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6));
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("ABANDON", "LTEXT_GUI_ABANDON_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5));
	menuItem->init();
	menu->addMenuItem(menuItem);
	m_menus[EPauseMenu] = menu;
}

void MenuManager::createGameOverMenu() {
	Menu* menu = new Menu("GAMEOVER_MENU", vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	menu->setTitle("GAME OVER");
	MenuItem* menuItem;
	menuItem = new MenuItem("RETRY", "LTEXT_GUI_RETRY_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6));
	menuItem->init();
	menu->addMenuItem(menuItem);
	menuItem = new MenuItem("MAIN_MENU", "LTEXT_GUI_ABANDON_MENU_ITEM", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5));
	menuItem->init();
	menu->addMenuItem(menuItem);
	m_menus[EGameOverMenu] = menu;
}