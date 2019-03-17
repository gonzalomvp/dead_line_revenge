#pragma once

#include "gui/controls/button.h"
#include "gui/controls/checkbox.h"
#include "gui/controls/slider.h"

#include <map>

class CMenu;

namespace menu { namespace constants {
	// Menu names
	extern const char* s_psMainMenuName;
	extern const char* s_psPlayMenuName;
	extern const char* s_psOptionsMenuName;
	extern const char* s_psPauseMenuName;
	extern const char* s_psGameOverMenuName;

	// Control names
	extern const char* s_psMainMenuButtonName;
	extern const char* s_psPlayMenuButtonName;
	extern const char* s_psOptionsMenuButtonName;
	extern const char* s_psEasyButtonName;
	extern const char* s_psMediumButtonName;
	extern const char* s_psHardButtonName;
	extern const char* s_psSettingsMusicCheckboxName;
	extern const char* s_psSettingsSFXCheckboxName;
	extern const char* s_psSettingsLanguageButtonName;
	extern const char* s_psSettingsVolumeSliderName;
	extern const char* s_psResumeButtonName;
	extern const char* s_psAbandonButtonName;
	extern const char* s_psRetryButtonName;
	extern const char* s_psExitButtonName;
	extern const char* s_psSettingsMusicLabelName;
	extern const char* s_psSettingsSFXLabelName;
	extern const char* s_psSettingsLanguageLabelName;
	extern const char* s_psSettingsVolumeLabelName;
	extern const char* s_psScoreLabelName;

	// File names
	extern const char* s_psButtonOffImage;
	extern const char* s_psButtonOnImage;
	extern const char* s_psCheckboxEnabledImage;
	extern const char* s_psCheckboxDisabledImage;
	extern const char* s_psSliderLeftButtonNormalImage;
	extern const char* s_psSliderLeftButtonPushImage;
	extern const char* s_psSliderRightButtonNormalImage;
	extern const char* s_psSliderRightButtonPushImage;
	extern const char* s_psSliderBarImage;
	extern const char* s_psSliderBallImage;
	extern const char* s_psMusicFile;

	// Texts
	extern const char* s_psPlayText;
	extern const char* s_psOptionsText;
	extern const char* s_psExitText;
	extern const char* s_psMusicText;
	extern const char* s_psSFXText;
	extern const char* s_psVolumeText;
	extern const char* s_psLanguageText;
	extern const char* s_psLanguageValueText;
	extern const char* s_psBackText;
	extern const char* s_psOnText;
	extern const char* s_psOffText;
	extern const char* s_psResumeText;
	extern const char* s_psAbandonText;
	extern const char* s_psRetryText;
	extern const char* s_psEasyText;
	extern const char* s_psMediumText;
	extern const char* s_psHardText;
	extern const char* s_psScoreText;
} }

class CMenuManager : public CButton::IListener, public CCheckbox::IListener, public Slider::IListener {
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

	virtual void onClick       (CButton* _pButton)     override;
	virtual void onClick       (CCheckbox* _pCheckbox) override;
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