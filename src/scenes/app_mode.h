#pragma once

#include "../gui/gui.h"
#include "../input/input_manager.h"
#include "../gui/menu.h"

class Menu;
class MenuItem;
class Text;

enum ModeId {
	MODE_NULL,
	MODE_MENU,
	MODE_GAME
};

class AppMode {
public:
	AppMode                    () {}
	virtual void   processInput() = 0;
	virtual void   run         () = 0;
	virtual void   render      () = 0;
	virtual void   init        () = 0;
	virtual void   deactivate  () = 0;
	virtual ModeId getModeId   () = 0;
};

class AppModeMenu : public AppMode {
public:
	AppModeMenu();
	virtual void   processInput();
	virtual void   run();
	virtual void   render();
	virtual void   init() {}
	virtual void   deactivate();
	virtual ModeId getModeId() { return MODE_MENU; }

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
	virtual void   processInput();
	virtual void   run         ();
	virtual void   render      ();
	virtual void   init        (){}
	virtual void   deactivate  ();
	virtual ModeId getModeId   (){ return MODE_GAME; }
	virtual bool   onEvent     (const IInputManager::Event&);
private:
	bool m_isPaused;
};
