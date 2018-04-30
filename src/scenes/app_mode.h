#pragma once

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

	virtual void  init        ();
	virtual void  processInput()                const;
	virtual void  run         (float deltaTime) const;
	virtual void  render      ()                const;
	virtual TMode getModeId   ()                const { return EMENU; }
};

//=============================================================================
// AppModeGame class
//=============================================================================
class AppModeGame : public AppMode {
public:
	AppModeGame (uint16_t level) : m_level(level) {}
	virtual ~AppModeGame();

	virtual void  init        ();
	virtual void  processInput()                const;
	virtual void  run         (float deltaTime) const;
	virtual void  render      ()                const;
	virtual TMode getModeId   ()                const { return EGAME; }
private:
	uint16_t m_level;
};
