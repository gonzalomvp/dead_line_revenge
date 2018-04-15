#pragma once

#include <vector>

class MenuItem;

class Menu {
public:
	static Menu* createMainMenu();
	static Menu* createPlayMenu();
	//static Menu* createOptionsMenu();

	void run();
	void selectPrevious();
	void selectNext();
	void activate();
	void deactivate();
	void setSelectedItem(int newOption);
	int  getSelectedItem() { return m_seletedItem; }
private:
	//Menu();

	int m_seletedItem;
	std::vector<MenuItem*> m_menuItems;
};