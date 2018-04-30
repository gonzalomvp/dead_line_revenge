#pragma once

#include "gui.h"
#include <map>

class Text;

//=============================================================================
// MenuItem class
//=============================================================================
class MenuItem {
public:
	MenuItem(std::string name, std::string text, const vec2& pos) : m_name(name), m_text(text), m_pos(pos), m_hasFocus(false), m_selectedOption(0) {}
	~MenuItem();

	void init();
	void activate();
	void deactivate();
	void run();
	void nextOption();
	
	void        addOption(std::string option) { m_options.push_back(option); m_selectedOption = 0; }
	std::string getName  () const             { return m_name; }
	void        setFocus (bool hasFocus)      { m_hasFocus = hasFocus; }

private:
	std::string              m_name;
	std::string              m_text;
	vec2                     m_pos;
	Text*                    m_gfxText;
	bool                     m_hasFocus;
	size_t                   m_selectedOption;
	std::vector<std::string> m_options;
	
};

//=============================================================================
// Menu class
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
// MenuManager class
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