#include "menu.h"
#include "menu_item.h"

Menu::~Menu() {
	g_inputManager->unregisterEvent(this);
}

Menu* Menu::createMainMenu() {
	Menu* menu = new Menu();
	MenuItem* menuItem;
	menuItem = new MenuItem("PLAY_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_PLAY_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("OPTIONS", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_OPTIONS_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("EXIT", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_OPTIONS_EXIT_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menu->m_seletedItem = 0;
	menu->m_menuItems[menu->m_seletedItem]->setSelected(true);
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
	menu->m_menuItems[menu->m_seletedItem]->setSelected(true);
	menu->deactivate();
	return menu;
}

Menu* Menu::createOptionsMenu() {
	Menu* menu = new Menu();
	MenuItem* menuItem;
	menuItem = new MenuItem("SETTINGS_MUSIC", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_MUSIC_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("SETTINGS_SFX", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_SFX_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("SETTINGS_LANGUAGE", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_LANGUAGE_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("MAIN_MENU", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.3), "LTEXT_GUI_BACK_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menu->m_seletedItem = 0;
	menu->m_menuItems[menu->m_seletedItem]->setSelected(true);
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
		m_menuItems[m_seletedItem]->setSelected(false);
		m_seletedItem = newOption;
		m_menuItems[m_seletedItem]->setSelected(true);
	}
}

void Menu::selectPrevious() {
	m_menuItems[m_seletedItem]->setSelected(false);
	m_seletedItem--;
	if (m_seletedItem < 0)
		m_seletedItem = m_menuItems.size() - 1;
	m_menuItems[m_seletedItem]->setSelected(true);
}

void Menu::selectNext() {
	m_menuItems[m_seletedItem]->setSelected(false);
	m_seletedItem++;
	if (m_seletedItem >= m_menuItems.size())
		m_seletedItem = 0;
	m_menuItems[m_seletedItem]->setSelected(true);
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
					for (auto itListener = m_listeners.begin(); itListener != m_listeners.end(); ++itListener) {
						(*itListener)->onSelected(m_menuItems[m_seletedItem]);
					}
					break;
			}
		}
	}

	return true;
}