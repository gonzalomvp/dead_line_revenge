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

AppModeMainMenu::AppModeMainMenu() {

	//ShowCursor(true);
	//m_menuItems.push_back(new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_PLAY_MENU_ITEM"));
	//m_menuItems.push_back(new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_OPTIONS_MENU_ITEM"));
	//m_menuItems.push_back(new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_OPTIONS_EXIT_ITEM"));
	//m_seletedItem = 0;
	//m_menuItems[m_seletedItem]->setSelected(true);
	m_mainMenu = Menu::createMainMenu();
	m_playMenu = Menu::createPlayMenu();
	m_activeMenu = m_mainMenu;
	g_inputManager->registerEvent(this, IInputManager::TEvent::EQuit, 0);
}


//ver por qué no funciona con el destructor. Seguramente porque el destructor no es virtual
void AppModeMainMenu::deactivate() {
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		delete *itControls;
	}
	g_inputManager->unregisterEvent(this);
	g_inputManager->clearListeners();
	g_graphicsEngine->removeAllGfxEntities();
}

//rehacer
void AppModeMainMenu::processInput() {
	g_inputManager->processInput();
}

void AppModeMainMenu::run() {
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->run();
	}
	m_activeMenu->run();
}

void AppModeMainMenu::render() {
	//glClear(GL_COLOR_BUFFER_BIT);
	//for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
	//	(*itControls)->run();
	//}
	//SYS_Show();
	g_graphicsEngine->render();
}


bool AppModeMainMenu::onEvent(const IInputManager::Event& event) {
	IInputManager::TEvent eventType = event.getType();

	if (eventType == IInputManager::TEvent::EKey) {
		const IInputManager::KeyEvent keyEvent = *static_cast<const IInputManager::KeyEvent*>(&event);
		if (keyEvent.action == keyEvent.KeyPressed) {
			switch (keyEvent.key)
			{
			case VK_UP:
				m_activeMenu->selectPrevious();
				break;
			case VK_DOWN:
				m_activeMenu->selectNext();
				break;
			case VK_RETURN:
				switch (m_activeMenu->getSelectedItem())
				{
				case 0:
					//m_activeMenu->deactivate();
					//m_playMenu->activate();
					//m_activeMenu = m_playMenu;
					g_appManager->switchMode(MODE_MENU);
					break;
				case 1:
					g_appManager->switchMode(MODE_OPTIONSMENU);
					break;
				case 2:
					exit(0);
					break;
				}

				
				break;
			}
		}
	}

	return true;
}

//=============================================================================
// AppModeOptionsMenu class

AppModeOptionsMenu::AppModeOptionsMenu() {
	MenuItem* menuItem;
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_MUSIC_MENU_ITEM");
	menuItem->init();
	m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_SFX_MENU_ITEM");
	menuItem->init();
	m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_LANGUAGE_MENU_ITEM");
	menuItem->init();
	m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.3), "LTEXT_GUI_BACK_MENU_ITEM");
	menuItem->init();
	m_menuItems.push_back(menuItem);

	m_seletedItem = 0;
	m_menuItems[m_seletedItem]->setSelected(true);

	g_inputManager->registerEvent(this, IInputManager::TEvent::EQuit, 0);
	//ShowCursor(true);
}


//ver por qué no funciona con el destructor. Seguramente porque el destructor no es virtual
void AppModeOptionsMenu::deactivate() {
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		delete *itControls;
	}
	g_inputManager->unregisterEvent(this);
	g_inputManager->clearListeners();
	g_graphicsEngine->removeAllGfxEntities();
}

//rehacer
void AppModeOptionsMenu::processInput() {
	g_inputManager->processInput();
}

void AppModeOptionsMenu::run() {
	if (g_settings.music)
		m_menuItems[0]->setValue("LTEXT_GUI_MENU_ITEM_ON");
	else
		m_menuItems[0]->setValue("LTEXT_GUI_MENU_ITEM_OFF");

	if (g_settings.sfx)
		m_menuItems[1]->setValue("LTEXT_GUI_MENU_ITEM_ON");
	else
		m_menuItems[1]->setValue("LTEXT_GUI_MENU_ITEM_OFF");

	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->run();
	}
}

void AppModeOptionsMenu::render() {
	//glClear(GL_COLOR_BUFFER_BIT);
	//for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
	//	(*itControls)->run();
	//}
	//SYS_Show();
	g_graphicsEngine->render();
}


bool AppModeOptionsMenu::onEvent(const IInputManager::Event& event) {
	IInputManager::TEvent eventType = event.getType();

	if (eventType == IInputManager::TEvent::EKey) {
		const IInputManager::KeyEvent keyEvent = *static_cast<const IInputManager::KeyEvent*>(&event);
		if (keyEvent.action == keyEvent.KeyPressed) {
			switch (keyEvent.key)
			{
			case VK_UP:
				m_menuItems[m_seletedItem]->setSelected(false);
				m_seletedItem--;
				if (m_seletedItem < 0)
					m_seletedItem = m_menuItems.size() - 1;
				m_menuItems[m_seletedItem]->setSelected(true);
				break;
			case VK_DOWN:
				m_menuItems[m_seletedItem]->setSelected(false);
				m_seletedItem++;
				if (m_seletedItem >= m_menuItems.size())
					m_seletedItem = 0;
				m_menuItems[m_seletedItem]->setSelected(true);
				break;
			case VK_RETURN:
				switch (m_seletedItem)
				{
				case 0:
					g_settings.music = !g_settings.music;
					break;
				case 1:
					g_settings.sfx = !g_settings.sfx;
					break;
				case 2:
					if (g_settings.language == EEnglish)
						g_settings.language = ESpanish;
					else if (g_settings.language == ESpanish)
						g_settings.language = EEnglish;
					g_stringManager->loadLanguage(g_settings.language);
					break;
				case 3:
					g_appManager->switchMode(MODE_MAINMENU);
					break;
				}


				break;
			}
		}
	}

	return true;
}

//=============================================================================
// AppModeMenu class

AppModeMenu::AppModeMenu() {
	/*Button* bLevel1 = new Button("bLevel1", vmake(100, 300), vmake(80, 31), "data/buttonOn.png", "data/buttonOff.png");
	bLevel1->addListener(this);
	m_controls.push_back(bLevel1);
	Button* bLevel2 = new Button("bLevel2", vmake(100, 200), vmake(80, 31), "data/buttonOn.png", "data/buttonOff.png");
	bLevel2->addListener(this);
	m_controls.push_back(bLevel2);
	Button* bLevel3 = new Button("bLevel3", vmake(100, 100), vmake(80, 31), "data/buttonOn.png", "data/buttonOff.png");
	bLevel3->addListener(this);
	m_controls.push_back(bLevel3);*/
	//ShowCursor(true);
}


//ver por qué no funciona con el destructor. Seguramente porque el destructor no es virtual
void AppModeMenu::deactivate() {
	g_graphicsEngine->removeAllGfxEntities();
	for (auto itControls = m_controls.begin(); itControls != m_controls.end(); ++itControls) {
		delete *itControls;
	}
}

void AppModeMenu::processInput() {
	g_inputManager->processInput();
	if (SYS_KeyPressed('1')) {
		g_appManager->switchMode(MODE_GAME, 1);
	}
	else if (SYS_KeyPressed('2')) {
		g_appManager->switchMode(MODE_GAME, 2);
	}
	else if (SYS_KeyPressed('3')) {
		g_appManager->switchMode(MODE_GAME, 3);
	}
	else if (!SYS_KeyPressed(VK_ESCAPE)) {
		m_keyReleased = true;
	}
	else if (SYS_KeyPressed(VK_ESCAPE) && m_keyReleased) {
		g_appManager->switchMode(MODE_MAINMENU);
	}
}

void AppModeMenu::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	FONT_DrawString(vmake(SCR_WIDTH / 2 - 10 * 16, SCR_HEIGHT * 0.6), "PRESS 1 FOR LEVEL 1");
	FONT_DrawString(vmake(SCR_WIDTH / 2 - 10 * 16, SCR_HEIGHT * 0.5), "PRESS 2 FOR LEVEL 2");
	FONT_DrawString(vmake(SCR_WIDTH / 2 - 10 * 16, SCR_HEIGHT * 0.4), "PRESS 3 FOR LEVEL 3");
	FONT_DrawString(vmake(SCR_WIDTH / 2 - 11 * 16, SCR_HEIGHT * 0.1), "PRESS ESC TO EXIT GAME");
	SYS_Show();
	//g_graphicsEngine->render();
}

void AppModeMenu::onClick(Button* button) {
	std::string bName = button->getName();
	if (bName == "bLevel1") {
		g_appManager->switchMode(MODE_GAME, 1);
	}
	else if (bName == "bLevel2") {
		g_appManager->switchMode(MODE_GAME, 2);
	}
	else if (bName == "bLevel3") {
		g_appManager->switchMode(MODE_GAME, 3);
	}
}

//=============================================================================
// AppModeGame class

AppModeGame::AppModeGame(int level) {
	g_world = new World(level);
	m_isPaused = false;
	g_inputManager->registerEvent(this, IInputManager::TEvent::EQuit, 0);
	ShowCursor(false);

	//prueba ingame menu
	m_PauseText = new Text("");
	g_graphicsEngine->addGfxEntity(m_PauseText);

	m_menuItems.push_back(new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_RESUME_MENU_ITEM"));
	m_menuItems.push_back(new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_ABANDON_MENU_ITEM"));
	m_seletedItem = 0;
	m_menuItems[m_seletedItem]->setSelected(true);
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->deactivate();
	}
}

void AppModeGame::deactivate() {
	g_inputManager->unregisterEvent(this);
	g_inputManager->clearListeners();
	g_graphicsEngine->removeAllGfxEntities();
	ShowCursor(true);
	delete g_world;
}

void AppModeGame::processInput() {
	g_inputManager->processInput();
}

void AppModeGame::run() {	
	
	if (!m_isPaused) {
		g_world->run();
	}
	else {
		for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
			(*itControls)->run();
		}
	}
}

void AppModeGame::render() {
	g_graphicsEngine->render();
}

bool AppModeGame::onEvent(const IInputManager::Event& event) {
	IInputManager::TEvent eventType = event.getType();

	if (eventType == IInputManager::TEvent::EQuit) {
		g_appManager->switchMode(MODE_MENU);
	}

	else if (eventType == IInputManager::TEvent::EPause) {
		m_isPaused = !m_isPaused;
		ShowCursor(m_isPaused);

		if (m_isPaused) {
			g_world->getPlayer()->deactivate();
			m_PauseText->setText("PAUSE");
			for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
				(*itControls)->activate();
			}
			m_menuItems[m_seletedItem]->setSelected(false);
			m_seletedItem = 0;
			m_menuItems[m_seletedItem]->setSelected(true);
		}
		else {
			g_world->getPlayer()->activate();
			m_PauseText->setText("");
			for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
				(*itControls)->deactivate();
			}
		}

		

	}
	else if (eventType == IInputManager::TEvent::EKey && m_isPaused) {
		const IInputManager::KeyEvent keyEvent = *static_cast<const IInputManager::KeyEvent*>(&event);
		if (keyEvent.action == keyEvent.KeyPressed) {
			switch (keyEvent.key)
			{
			case VK_UP:
				m_menuItems[m_seletedItem]->setSelected(false);
				m_seletedItem--;
				if (m_seletedItem < 0)
					m_seletedItem = m_menuItems.size() - 1;
				m_menuItems[m_seletedItem]->setSelected(true);
				break;
			case VK_DOWN:
				m_menuItems[m_seletedItem]->setSelected(false);
				m_seletedItem++;
				if (m_seletedItem >= m_menuItems.size())
					m_seletedItem = 0;
				m_menuItems[m_seletedItem]->setSelected(true);
				break;
			case VK_RETURN:
				switch (m_seletedItem)
				{
				case 0:
					IInputManager::KeyEvent pauseEvent;
					pauseEvent.setType(IInputManager::EPause);
					//g_inputManager->addEvent();
					onEvent(pauseEvent);
					break;
				case 1:
					g_appManager->switchMode(MODE_MENU);
					break;
				}


				break;
			}
		}
	}

	return true;
}