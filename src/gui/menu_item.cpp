#include "menu_item.h"
#include "string_manager.h"

void MenuItem::init() {
	m_active = true;
	g_graphicsEngine->addGfxEntity(m_gfxText);
}

void MenuItem::run() {
	std::string textToDraw = g_stringManager->getText(m_text);
	if (m_selectedOption != -1)
		textToDraw += " " + g_stringManager->getText(m_options[m_selectedOption]);
	if (m_hasFocus)
		textToDraw = "- " + textToDraw + " -";
	
	m_gfxText->setPos(vmake(m_pos.x - (textToDraw.length() / 2.0f * 16), m_pos.y));
	m_gfxText->setText(textToDraw.c_str());

	//FONT_DrawString(vmake(m_pos.x - (textToDraw.length() / 2.0f * 16), m_pos.y), textToDraw.c_str());
}

void MenuItem::nextOption() {
	if (m_selectedOption != -1) {
		++m_selectedOption;
		if (m_selectedOption >= m_options.size())
			m_selectedOption = 0;
	}
}

void MenuItem::activate() {
	m_active = true;
	m_gfxText->activate();
}

void MenuItem::deactivate() {
	m_active = false;
	m_gfxText->deactivate();
}