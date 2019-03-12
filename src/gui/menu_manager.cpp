#include "common/stdafx.h"
#include "menu_manager.h"

#include "engine/sound_engine.h"
#include "gui/controls/label.h"
#include "gui/controls/menu.h"
#include "gui/string_manager.h"
#include "scenes/scene_manager.h"
#include "scenes/world.h"

namespace menu { namespace constants {
	// Menu names
	const char* s_psMainMenuName     = "MAIN_MENU";
	const char* s_psPlayMenuName     = "PLAY_MENU";
	const char* s_psOptionsMenuName  = "OPTIONS_MENU";
	const char* s_psPauseMenuName    = "PAUSE_MENU";
	const char* s_psGameOverMenuName = "GAMEOVER_MENU";

	// Control names
	const char* s_psMainMenuButtonName         = "MAIN_MENU_BTN";
	const char* s_psPlayMenuButtonName         = "PLAY_MENU_BTN";
	const char* s_psOptionsMenuButtonName      = "OPTIONS_MENU_BTN";
	const char* s_psEasyButtonName             = "EASY_BTN";
	const char* s_psMediumButtonName           = "MEDIUM_BTN";
	const char* s_psHardButtonName             = "HARD_BTN";
	const char* s_psSettingsMusicCheckboxName  = "SETTINGS_MUSIC_CHK";
	const char* s_psSettingsSFXCheckboxName    = "SETTINGS_SFX_CHK";
	const char* s_psSettingsLanguageButtonName = "SETTINGS_LANGUAGE_BTN";
	const char* s_psSettingsVolumeSliderName   = "SETTINGS_VOLUME_SLD";
	const char* s_psResumeButtonName           = "RESUME_BTN";
	const char* s_psAbandonButtonName          = "ABANDON_BTN";
	const char* s_psRetryButtonName            = "RETRY_BTN";
	const char* s_psExitButtonName             = "EXIT_BTN";
	const char* s_psSettingsMusicLabelName     = "SETTINGS_MUSIC_LBL";
	const char* s_psSettingsSFXLabelName       = "SETTINGS_SFX_LBL";
	const char* s_psSettingsLanguageLabelName  = "SETTINGS_LANGUAGE_LBL";
	const char* s_psSettingsVolumeLabelName    = "SETTINGS_VOLUME_LBL";
	const char* s_psScoreLabelName             = "SCORE_LBL";

	// File names
	const char* s_psButtonOffImage               = "data/ui/buttonOff.png";
	const char* s_psButtonOnImage                = "data/ui/buttonOn.png";
	const char* s_psCheckboxEnabledImage         = "data/ui/CheckBox_enabled.png";
	const char* s_psCheckboxDisabledImage        = "data/ui/CheckBox_disabled.png";
	const char* s_psSliderLeftButtonNormalImage  = "data/ui/Slider_Left_Normal.png";
	const char* s_psSliderLeftButtonPushImage    = "data/ui/Slider_Left_Push.png";
	const char* s_psSliderRightButtonNormalImage = "data/ui/Slider_Right_Normal.png";
	const char* s_psSliderRightButtonPushImage   = "data/ui/Slider_Right_Push.png";
	const char* s_psSliderBarImage               = "data/ui/Slider_bar.png";
	const char* s_psSliderBallImage              = "data/ui/Slider_ball.png";
	const char* s_psMusicFile                    = "data/audio/menu.wav";

	// Texts
	const char* s_psPlayText          = "LTEXT_GUI_PLAY";
	const char* s_psOptionsText       = "LTEXT_GUI_OPTIONS";
	const char* s_psExitText          = "LTEXT_GUI_EXIT";
	const char* s_psMusicText         = "LTEXT_GUI_MUSIC";
	const char* s_psSFXText           = "LTEXT_GUI_SFX";
	const char* s_psVolumeText        = "LTEXT_GUI_VOLUME";
	const char* s_psLanguageText      = "LTEXT_GUI_LANGUAGE";
	const char* s_psLanguageValueText = "LTEXT_GUI_LANGUAGE_VALUE";
	const char* s_psBackText          = "LTEXT_GUI_BACK";
	const char* s_psOnText            = "LTEXT_GUI_OFF";
	const char* s_psOffText           = "LTEXT_GUI_SFX";
	const char* s_psResumeText        = "LTEXT_GUI_RESUME";
	const char* s_psAbandonText       = "LTEXT_GUI_ABANDON";
	const char* s_psRetryText         = "LTEXT_GUI_RETRY";
	const char* s_psEasyText          = "LTEXT_GUI_EASY";
	const char* s_psMediumText        = "LTEXT_GUI_MEDIUM";
	const char* s_psHardText          = "LTEXT_GUI_HARD";
	const char* s_psScoreText         = "LTEXT_GUI_SCORE_MESSAGE";
} }

using namespace menu::constants;

CMenuManager::~CMenuManager() {
	for (auto itMenu = m_mMenus.begin(); itMenu != m_mMenus.end(); ++itMenu) {
		DELETE(itMenu->second);
	}
}

void CMenuManager::init() {
	createMainMenu();
	createPlayMenu();
	createOptionsMenu();
	createPauseMenu();
	createGameOverMenu();

	for (auto itMenu = m_mMenus.begin(); itMenu != m_mMenus.end(); ++itMenu) {
		itMenu->second->deactivate();
	}
}

void CMenuManager::run(float _fDeltaTime) {
	if (m_pActiveMenu) {
		m_pActiveMenu->run(_fDeltaTime);
	}
}

void CMenuManager::activateMenu(EMenu _eMenu) {
	ASSERT(m_mMenus.count(_eMenu));

	if (m_pActiveMenu) {
		m_pActiveMenu->deactivate();
		ShowCursor(false);
	}

	m_pActiveMenu = m_mMenus[_eMenu];
	m_pActiveMenu->activate();
	ShowCursor(true);
}

void CMenuManager::deactivateMenu() {
	if (m_pActiveMenu) {
		m_pActiveMenu->deactivate();
		m_pActiveMenu = nullptr;
	}
	ShowCursor(false);
}

CMenu* CMenuManager::getMenu(EMenu _eMenu) {
	ASSERT(m_mMenus.count(_eMenu));

	return m_mMenus[_eMenu];
}

void CMenuManager::onClick(CButton* _pButton) {
	ASSERT(_pButton);
	ASSERT(g_pSceneManager && g_pInputManager && g_pStringManager);

	if (_pButton->getName() == s_psPlayMenuButtonName) {
		activateMenu(EPlayMenu);
	}
	else if (_pButton->getName() == s_psOptionsMenuButtonName) {
		activateMenu(EOptionsMenu);
	}
	else if (_pButton->getName() == s_psMainMenuButtonName) {
		activateMenu(EMainMenu);
		g_pSceneManager->switchScene(IScene::EMENU);
	}
	else if (_pButton->getName() == s_psEasyButtonName) {
		g_pSceneManager->switchScene(IScene::EGAME, 1);
	}
	else if (_pButton->getName() == s_psMediumButtonName) {
		g_pSceneManager->switchScene(IScene::EGAME, 2);
	}
	else if (_pButton->getName() == s_psHardButtonName) {
		g_pSceneManager->switchScene(IScene::EGAME, 3);
	}
	else if (_pButton->getName() == s_psSettingsLanguageButtonName) {
		if (g_settings.language == EEnglish) {
			g_settings.language = ESpanish;
		}
		else {
			g_settings.language = EEnglish;
		}
		g_pStringManager->loadLanguage(g_settings.language);
	}
	else if (_pButton->getName() == s_psResumeButtonName) {
		IInputManager::CEvent* pPauseEvent = NEW(IInputManager::CEvent, IInputManager::EEventType::EPause);
		g_pInputManager->addEvent(pPauseEvent);
	}
	else if (_pButton->getName() == s_psAbandonButtonName) {
		g_pSceneManager->switchScene(IScene::EMENU);
	}
	else if (_pButton->getName() == s_psRetryButtonName) {
		ASSERT(g_pWorld);
		deactivateMenu();
		g_pWorld->init(g_pWorld->getLevel());
	}
	else if (_pButton->getName() == s_psExitButtonName) {
		exit(0);
	}
}

void CMenuManager::onClick(Checkbox* _pCheckbox) {
	ASSERT(_pCheckbox);
	ASSERT(g_pSoundEngine);

	if (_pCheckbox->getName() == s_psSettingsSFXCheckboxName) {
		g_settings.sfx = _pCheckbox->isChecked();
	}
	else if (_pCheckbox->getName() == s_psSettingsMusicCheckboxName) {
		g_settings.music = !g_settings.music;
		if (g_settings.music) {
			g_pSoundEngine->playMusic(s_psMusicFile);
		}
		else {
			g_pSoundEngine->stopMusic();
		}
	}
}

void CMenuManager::onValueChange(Slider* _pSlider) {
	ASSERT(_pSlider);
	ASSERT(g_pSoundEngine);

	if (_pSlider->getName() == s_psSettingsVolumeSliderName) {
		g_settings.volume = _pSlider->getValue();
		g_pSoundEngine->setVolume(g_settings.volume);
	}
}

void CMenuManager::createMainMenu() {
	CMenu* menu = NEW(CMenu, s_psMainMenuName, vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	m_mMenus[EMainMenu] = menu;

	CButton* button = NEW(CButton, s_psPlayMenuButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), vmake(150, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psPlayText);
	button->addListener(this);
	menu->addControl(button);

	button = NEW(CButton, s_psOptionsMenuButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), vmake(150, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psOptionsText);
	button->addListener(this);
	menu->addControl(button);

	button = NEW(CButton, s_psExitButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.4), vmake(150, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psExitText);
	button->addListener(this);
	menu->addControl(button);
}

void CMenuManager::createPlayMenu() {
	CMenu* menu = NEW(CMenu, s_psPlayMenuName, vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	m_mMenus[EPlayMenu] = menu;

	CButton* button = NEW(CButton, s_psEasyButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.65), vmake(150, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psEasyText);
	button->addListener(this);
	menu->addControl(button);

	button = NEW(CButton, s_psMediumButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.55), vmake(150, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psMediumText);
	button->addListener(this);
	menu->addControl(button);

	button = NEW(CButton, s_psHardButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.45), vmake(150, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psHardText);
	button->addListener(this);
	menu->addControl(button);

	button = NEW(CButton, s_psMainMenuButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.35), vmake(150, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psBackText);
	button->addListener(this);
	menu->addControl(button);
}

void CMenuManager::createOptionsMenu() {
	CMenu* menu = NEW(CMenu, s_psOptionsMenuName, vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	m_mMenus[EOptionsMenu] = menu;

	Label* label = NEW(Label, s_psSettingsMusicLabelName, vmake(SCR_WIDTH / 2 - 100, SCR_HEIGHT * 0.8), vmake(0.0, 0.0), s_psMusicText);
	label->init();
	menu->addControl(label);

	Checkbox* checkbox = NEW(Checkbox, s_psSettingsMusicCheckboxName, vmake(SCR_WIDTH / 2 + 100, SCR_HEIGHT * 0.8), vmake(35, 35));
	checkbox->init(s_psCheckboxEnabledImage, s_psCheckboxDisabledImage, g_settings.music);
	checkbox->addListener(this);
	menu->addControl(checkbox);

	label = NEW(Label, s_psSettingsSFXLabelName, vmake(SCR_WIDTH / 2 - 100, SCR_HEIGHT * 0.65), vmake(0.0, 0.0), s_psSFXText);
	label->init();
	menu->addControl(label);

	checkbox = NEW(Checkbox, s_psSettingsSFXCheckboxName, vmake(SCR_WIDTH / 2 + 100, SCR_HEIGHT * 0.65), vmake(35, 35));
	checkbox->init(s_psCheckboxEnabledImage, s_psCheckboxDisabledImage, g_settings.sfx);
	checkbox->addListener(this);
	menu->addControl(checkbox);

	label = NEW(Label, s_psSettingsVolumeLabelName, vmake(SCR_WIDTH / 2 - 100, SCR_HEIGHT * 0.5), vmake(0.0, 0.0), s_psVolumeText);
	label->init();
	menu->addControl(label);

	Slider* slider = NEW(Slider, s_psSettingsVolumeSliderName, vmake(SCR_WIDTH / 2 + 100, SCR_HEIGHT * 0.5), vmake(100, 32));
	slider->init(s_psSliderLeftButtonNormalImage, s_psSliderLeftButtonPushImage, s_psSliderRightButtonNormalImage, s_psSliderRightButtonPushImage, s_psSliderBarImage, s_psSliderBallImage, g_settings.volume);
	slider->addListener(this);
	menu->addControl(slider);

	label = NEW(Label, s_psSettingsLanguageLabelName, vmake(SCR_WIDTH / 2 - 100, SCR_HEIGHT * 0.35), vmake(0.0, 0.0), s_psLanguageText);
	label->init();
	menu->addControl(label);

	CButton* button = NEW(CButton, s_psSettingsLanguageButtonName, vmake(SCR_WIDTH / 2 + 100, SCR_HEIGHT * 0.35), vmake(150, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psLanguageValueText);
	button->addListener(this);
	menu->addControl(button);
	
	button = NEW(CButton, s_psMainMenuButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.15), vmake(150, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psBackText);
	button->addListener(this);
	menu->addControl(button);
}

void CMenuManager::createPauseMenu() {
	CMenu* menu = NEW(CMenu, s_psPauseMenuName, vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	m_mMenus[EPauseMenu] = menu;

	CButton* button = NEW(CButton, s_psResumeButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), vmake(175, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psResumeText);
	button->addListener(this);
	menu->addControl(button);

	button = NEW(CButton, s_psAbandonButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), vmake(175, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psAbandonText);
	button->addListener(this);
	menu->addControl(button);
}

void CMenuManager::createGameOverMenu() {
	CMenu* menu = NEW(CMenu, s_psGameOverMenuName, vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), false);
	m_mMenus[EGameOverMenu] = menu;

	CButton* button = NEW(CButton, s_psRetryButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.6), vmake(175, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psRetryText);
	button->addListener(this);
	menu->addControl(button);

	button = NEW(CButton, s_psMainMenuButtonName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.5), vmake(175, 35));
	button->init(s_psButtonOffImage, s_psButtonOnImage, s_psAbandonText);
	button->addListener(this);
	menu->addControl(button);

	Label* pLabel = NEW(Label, s_psScoreLabelName, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.8), vmake(0.0, 0.0), "");
	pLabel->init();
	menu->addControl(pLabel);
}