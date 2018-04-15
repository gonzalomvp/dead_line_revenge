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

	void run();
	void selectPrevious();
	void selectNext();
	virtual void activate();
	virtual void deactivate();
	void setSelectedItem(int newOption);
	int  getSelectedItem() { return m_seletedItem; }
	void addListener(IListener* listener) { m_listeners.push_back(listener); }
	virtual bool onEvent(const IInputManager::Event&);
private:
	Menu() : Control(std::string(""), vmake(0,0), vmake(0, 0)) {}

	int m_seletedItem;
	std::vector<MenuItem*> m_menuItems;
	std::vector<IListener*> m_listeners;
};