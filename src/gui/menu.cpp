#include "../common/stdafx.h"
#include "menu.h"

#include "../engine/graphics_engine.h"
#include "../engine/sound_engine.h"
#include "../gui/label.h"
#include "../gui/string_manager.h"
#include "scenes/scene.h"
#include "scenes/scene_manager.h"
#include "../scenes/world.h"


//=============================================================================
// MenuItem class
//=============================================================================
MenuItem::~MenuItem() {
	DELETE(m_gfxText);
}

void MenuItem::init() {
	m_gfxText = NEW(Text, m_text, m_pos, 1);
	m_gfxText->deactivate();
	g_pGraphicsEngine->addGfxEntity(m_gfxText);
}

void MenuItem::activate() {
	m_gfxText->activate();
}

void MenuItem::deactivate() {
	m_gfxText->deactivate();
}

void MenuItem::run() {
	std::string textToDraw = g_pStringManager->getText(m_text);
	if (m_options.size() > 0) {
		textToDraw += " " + g_pStringManager->getText(m_options[m_selectedOption]);
	}
	if (m_hasFocus) {
		textToDraw = "- " + textToDraw + " -";
	}
	m_gfxText->setPos(vmake(m_pos.x - (textToDraw.length() / 2.0f * 16), m_pos.y));
	m_gfxText->setText(textToDraw.c_str());
}

void MenuItem::nextOption() {
	if (m_options.size() > 0) {
		++m_selectedOption;
		if (m_selectedOption >= m_options.size())
			m_selectedOption = 0;
	}
}

//=============================================================================
// Menu class
//=============================================================================
Menu::~Menu() {
	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EKeyDown);
	}
	for (auto itMenuItems = m_menuItems.begin(); itMenuItems != m_menuItems.end(); ++itMenuItems) {
		DELETE(*itMenuItems);
	}
	for (auto itControls = m_controls.begin(); itControls != m_controls.end(); ++itControls) {
		DELETE(*itControls);
	}
	if (m_title) {
		DELETE(m_title);
	}
}

void Menu::activate() {
	Control::activate();
	for (auto itMenuItems = m_menuItems.begin(); itMenuItems != m_menuItems.end(); ++itMenuItems) {
		(*itMenuItems)->activate();
	}
	for (auto itControls = m_controls.begin(); itControls != m_controls.end(); ++itControls) {
		(*itControls)->activate();
	}
	if (m_title) {
		m_title->activate();
	}
	setSelectedItem(0);
	//g_pInputManager->registerEvent(this, IInputManager::EEventType::EKeyDown);
}

void Menu::deactivate() {
	Control::deactivate();
	for (auto itMenuItems = m_menuItems.begin(); itMenuItems != m_menuItems.end(); ++itMenuItems) {
		(*itMenuItems)->deactivate();
	}
	for (auto itControls = m_controls.begin(); itControls != m_controls.end(); ++itControls) {
		(*itControls)->deactivate();
	}
	if (m_title) {
		m_title->deactivate();
	}
	g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EKeyDown);
}

void Menu::run(float _fDeltaTime) {
	if (!m_isActive)
		return;

	for (auto itControls = m_menuItems.begin(); itControls != m_menuItems.end(); ++itControls) {
		(*itControls)->run();
	}
	for (auto itControls = m_controls.begin(); itControls != m_controls.end(); ++itControls) {
		(*itControls)->run();
	}
}

bool Menu::onEvent(const IInputManager::CEvent& event) {
	IInputManager::EEventType eventType = event.getType();
	if (eventType == IInputManager::EEventType::EKeyDown) {
		const IInputManager::CKeyEvent keyEvent = *static_cast<const IInputManager::CKeyEvent*>(&event);
		switch (keyEvent.getKey()) {
			case VK_UP:
				selectPreviousItem();
				break;
			case VK_DOWN:
				selectNextItem();
				break;
			case VK_RETURN:
				m_menuItems[m_seletedItem]->nextOption();
				for (auto itListener = m_listeners.begin(); itListener != m_listeners.end(); ++itListener) {
					(*itListener)->onSelected(m_menuItems[m_seletedItem]);
				}
				break;
		}
	}
	return true;
}

void Menu::setSelectedItem(int newOption) {
	if (newOption >= 0 && newOption < static_cast<int>(m_menuItems.size())) {
		m_menuItems[m_seletedItem]->setFocus(false);
		m_seletedItem = newOption;
		m_menuItems[m_seletedItem]->setFocus(true);
	}
}

void Menu::setTitle(const std::string& title) {
	if (m_title) {
		m_title->setText(title);
	}
	else {
		m_title = NEW(Text, title, vmake(SCR_WIDTH / 2, SCR_HEIGHT * 0.8), 1);
		if (!m_isActive) {
			m_title->deactivate();
		}
		g_pGraphicsEngine->addGfxEntity(m_title);
	}
	m_title->setPos(vmake((SCR_WIDTH / 2) - (title.length() / 2.0f * 16), m_title->getPos().y));
}

void Menu::selectPreviousItem() {
	if (m_seletedItem >= 0 && m_seletedItem < m_menuItems.size()) {
		m_menuItems[m_seletedItem]->setFocus(false);
		m_seletedItem--;
		if (m_seletedItem < 0) {
			m_seletedItem = m_menuItems.size() - 1;
		}
		m_menuItems[m_seletedItem]->setFocus(true);
	}
}

void Menu::selectNextItem() {
	if (m_seletedItem >= 0 && m_seletedItem < m_menuItems.size()) {
		m_menuItems[m_seletedItem]->setFocus(false);
		m_seletedItem++;
		if (m_seletedItem >= static_cast<int>(m_menuItems.size())) {
			m_seletedItem = 0;
		}
		m_menuItems[m_seletedItem]->setFocus(true);
	}
}
