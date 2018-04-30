#pragma once

#include "gui.h"
#include <vector>
#include <map>

class MenuItem;
class Text;

//=============================================================================
// Component Menu
//=============================================================================
class Menu : public Control {

public:
	class IListener {
	public:
		virtual void onSelected(MenuItem* menuItem) = 0;
	};

	Menu(std::string name, const vec2& pos, const vec2& size, bool isActive) : Control(name, pos, size, isActive), m_seletedItem(0), m_title(nullptr) {}
	~Menu();

	virtual void activate  ();
	virtual void deactivate();
	virtual void run       ();

	virtual bool onEvent     (const IInputManager::Event& event);

	void         addListener(IListener* listener)  { m_listeners.push_back(listener); }
	void         addMenuItem(MenuItem* menuItem)   { m_menuItems.push_back(menuItem); }

	MenuItem* getSelectedItem() const { return m_menuItems[m_seletedItem]; }
	void      setSelectedItem(int newOption);
	void      setTitle       (const char* title);
private:
	void selectPreviousItem();
	void selectNextItem    ();

	int                     m_seletedItem;
	Text*                   m_title;
	std::vector<MenuItem*>  m_menuItems;
	std::vector<IListener*> m_listeners;
};

//=============================================================================
// Component MenuManager
//=============================================================================
class MenuManager : public Menu::IListener {
public:
	enum TMenu {
		EMainMenu,
		EOptionsMenu,
		EPlayMenu,
		EPauseMenu,
		EGameOverMenu,
	};

	~MenuManager();

	void         init          ();
	void         run           ();
	void         activateMenu  (TMenu menu);
	void         deactivateMenu();
	virtual void onSelected    (MenuItem* menuItem);
	Menu*        getMenu       (TMenu menu);
private:
	void createMainMenu    ();
	void createPlayMenu    ();
	void createOptionsMenu ();
	void createPauseMenu   ();
	void createGameOverMenu();

	Menu*                  m_activeMenu;
	std::map<TMenu, Menu*> m_menus;
};