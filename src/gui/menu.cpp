#include "menu.h"
#include "menu_item.h"

Menu* Menu::createMainMenu() {
	Menu* menu = new Menu();
	MenuItem* menuItem;
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_PLAY_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_OPTIONS_MENU_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_OPTIONS_EXIT_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menu->m_seletedItem = 0;
	menu->m_menuItems[menu->m_seletedItem]->setSelected(true);
	return menu;
}

Menu* Menu::createPlayMenu() {
	Menu* menu = new Menu();
	MenuItem* menuItem;
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), "LTEXT_GUI_PLAY_EASY_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), "LTEXT_GUI_PLAY_MEDIUM_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menuItem = new MenuItem("", vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), "LTEXT_GUI_PLAY_HARD_ITEM");
	menuItem->init();
	menu->m_menuItems.push_back(menuItem);
	menu->m_seletedItem = 0;
	menu->m_menuItems[menu->m_seletedItem]->setSelected(true);
	return menu;
}

void Menu::run() {
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->run();
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
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->activate();
	}
}

void Menu::deactivate() {
	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->deactivate();
	}
}