#pragma once

#include "../common/stdafx.h"
#include "../input/input_manager.h"

class Menu;
class MenuItem;
class Text;

//=============================================================================
// AppMode class
//=============================================================================
class AppMode {
public:
	enum TMode {
		ENULL,
		EMENU,
		EGAME,
	};

	AppMode                    () {}
	virtual ~AppMode() {}
	virtual void   processInput() = 0;
	virtual void   run         (float deltaTime) = 0;
	virtual void   render      () = 0;
	virtual void   init        () = 0;
	virtual TMode getModeId   () = 0;
};

class AppModeMenu : public AppMode {
public:
	AppModeMenu();
	virtual ~AppModeMenu();
	virtual void   processInput();
	virtual void   run(float deltaTime);
	virtual void   render();
	virtual void   init() {}
	virtual TMode getModeId() { return EMENU; }

private:
	Menu* m_mainMenu;
	Menu* m_playMenu;
	Menu* m_optionsMenu;
	Menu* m_activeMenu;
	std::vector<MenuItem*> m_menuItems;
	int m_seletedItem;
};

class AppModeGame : public AppMode, public IInputManager::IListener {
public:
	AppModeGame                (int level);
	virtual ~AppModeGame();
	virtual void   processInput();
	virtual void   run         (float deltaTime);
	virtual void   render      ();
	virtual void   init        (){}
	virtual TMode getModeId   (){ return EGAME; }
	virtual bool   onEvent     (const IInputManager::Event&);
private:
	bool m_isPaused;
};
