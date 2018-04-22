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

	virtual ~AppMode() {}

	virtual void  init        ()                      = 0;
	virtual void  processInput()                const = 0;
	virtual void  run         (float deltaTime) const = 0;
	virtual void  render      ()                const = 0;
	virtual TMode getModeId   ()                const = 0;
};

//=============================================================================
// AppModeMenu class
//=============================================================================
class AppModeMenu : public AppMode {
public:
	virtual ~AppModeMenu();

	virtual void  init        ();
	virtual void  processInput()                const;
	virtual void  run         (float deltaTime) const;
	virtual void  render      ()                const;
	virtual TMode getModeId   ()                const { return EMENU; }
};

//=============================================================================
// AppModeGame class
//=============================================================================
class AppModeGame : public AppMode, public IInputManager::IListener {
public:
	AppModeGame (uint16_t level) : m_level(level), m_isPaused(false) {}
	virtual ~AppModeGame();

	virtual void  init        ();
	virtual void  processInput()                const;
	virtual void  run         (float deltaTime) const;
	virtual void  render      ()                const;
	virtual TMode getModeId   ()                const { return EGAME; }
	virtual bool  onEvent     (const IInputManager::Event& event);
private:
	uint16_t m_level;
	bool     m_isPaused;
};
