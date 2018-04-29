#pragma once

#include "gui.h"
#include <vector>
#include <map>

class MenuItem;
class Text;

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
	static Menu* createGameOverMenu();

	void run();
	void selectPrevious();
	void selectNext();
	virtual void activate();
	virtual void deactivate();
	void setSelectedItem(size_t newOption);
	int  getSelectedItem() { return m_seletedItem; }
	void addListener(IListener* listener) { m_listeners.push_back(listener); }
	virtual bool onEvent(const IInputManager::Event&);
	void setTitle(const char* title);

	std::vector<MenuItem*> m_menuItems;
private:
	Menu() : Control(std::string(""), vmake(0,0), vmake(0, 0)), m_title(nullptr) {}

	size_t m_seletedItem;

	std::vector<IListener*> m_listeners;
	Text* m_title;
};

class MenuManager : public Menu::IListener {
public:
	enum TMenu {
		EMainMenu,
		EOptionsMenu,
		EPlayMenu,
		EPauseMenu,
		EGameOverMenu,
	};

	MenuManager();
	~MenuManager() {}
	void run();
	void activateMenu(TMenu menu);
	void deactivateMenu();
	virtual void onSelected(MenuItem* menuItem);
	Menu* getMenu(TMenu menu);
private:
	std::map<TMenu, Menu*> m_menus;
	//Menu* m_mainMenu;
	//Menu* m_playMenu;
	//Menu* m_optionsMenu;
	//Menu* m_pauseMenu;
	Menu* m_activeMenu;
	//Menu* m_gameOverMenu;
};