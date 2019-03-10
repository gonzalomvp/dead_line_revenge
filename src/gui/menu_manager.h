#pragma once

#include "button.h"
#include "checkbox.h"
#include "slider.h"

#include <map>

class CMenu;

class CMenuManager : public Button::IListener, public Checkbox::IListener, public Slider::IListener {
public:
	enum EMenu {
		EMainMenu,
		EOptionsMenu,
		EPlayMenu,
		EPauseMenu,
		EGameOverMenu,
	};

	CMenuManager() : m_pActiveMenu(nullptr) {}
	~CMenuManager();

	void         init          ();
	void         run           (float _fDeltaTime);
	void         activateMenu  (EMenu _eMenu);
	void         deactivateMenu();
	CMenu*       getMenu       (EMenu _eMenu);

	virtual void onClick       (Button* _pButton)     override;
	virtual void onClick       (Checkbox* _pCheckbox) override;
	virtual void onValueChange (Slider* _pSlider)     override;


private:
	void createMainMenu    ();
	void createPlayMenu    ();
	void createOptionsMenu ();
	void createPauseMenu   ();
	void createGameOverMenu();

	CMenu*                  m_pActiveMenu;
	std::map<EMenu, CMenu*> m_mMenus;
};