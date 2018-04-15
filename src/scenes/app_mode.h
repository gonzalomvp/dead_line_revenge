#pragma once

#include "../gui/gui.h"
#include "../input/input_manager.h"

class MenuItem;
class Text;

enum ModeId {
	MODE_NULL,
	MODE_MAINMENU,
	MODE_OPTIONSMENU,
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

class AppModeMainMenu : public AppMode, public IInputManager::IListener {
public:
	AppModeMainMenu();
	virtual void   processInput();
	virtual void   run();
	virtual void   render();
	virtual void   init() {}
	virtual void   deactivate();
	virtual ModeId getModeId() { return MODE_MAINMENU; }
	virtual bool   onEvent(const IInputManager::Event&);

private:
	std::vector<MenuItem*> m_menuItems;
	int m_seletedItem;
};

class AppModeOptionsMenu : public AppMode, public IInputManager::IListener {
public:
	AppModeOptionsMenu();
	virtual void   processInput();
	virtual void   run();
	virtual void   render();
	virtual void   init() {}
	virtual void   deactivate();
	virtual ModeId getModeId() { return MODE_OPTIONSMENU; }
	virtual bool   onEvent(const IInputManager::Event&);

private:
	std::vector<MenuItem*> m_menuItems;
	int m_seletedItem;
};

class AppModeMenu : public AppMode, public Button::IListener {
public:
	AppModeMenu                ();
	virtual void   processInput();
	virtual void   run         (){}
	virtual void   render      ();
	virtual void   init        (){}
	virtual void   deactivate  ();
	virtual ModeId getModeId   (){ return MODE_MENU; }
	virtual void   onClick     (Button* button);

private:
	bool m_keyReleased = false;
	std::vector<Control*> m_controls;
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
	std::vector<MenuItem*> m_menuItems;
	int m_seletedItem;
	bool m_isPaused;
	Text* m_PauseText;
};
