#include "../common/stdafx.h"
#include "menu_item.h"

#include "gui/menu.h"
#include "gui/control.h"
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

