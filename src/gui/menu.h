#pragma once

#include "gui.h"
#include <vector>

class MenuItem;

class Menu : public Control {

public:
	class IListener {
	public:
		virtual void onSelected(MenuItem* menuItem) = 0;
	};

	~Menu();

	static Menu* createMainMenu();
	static Menu* createPlayMenu();
	static Menu* createOptionsMenu();
	static Menu* createPauseMenu();

	void run();
	void selectPrevious();
	void selectNext();
	virtual void activate();
	virtual void deactivate();
	void setSelectedItem(int newOption);
	int  getSelectedItem() { return m_seletedItem; }
	void addListener(IListener* listener) { m_listeners.push_back(listener); }
	virtual bool onEvent(const IInputManager::Event&);

	std::vector<MenuItem*> m_menuItems;
private:
	Menu() : Control(std::string(""), vmake(0,0), vmake(0, 0)) {}

	int m_seletedItem;

	std::vector<IListener*> m_listeners;
};

class MenuManager : public Menu::IListener {
public:
	enum TMenu {
		EMainMenu,
		EOptionsMenu,
		EPlayMenu,
		EPauseMenu,
	};

	MenuManager();
	~MenuManager() {}
	void run();
	void activateMenu(TMenu menu);
	void deactivateMenu();
	virtual void onSelected(MenuItem* menuItem);
private:
	Menu* m_mainMenu;
	Menu* m_playMenu;
	Menu* m_optionsMenu;
	Menu* m_pauseMenu;
	Menu* m_activeMenu;
};