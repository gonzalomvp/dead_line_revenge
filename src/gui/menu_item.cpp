#include "menu_item.h"
#include "string_manager.h"

void MenuItem::init() {
	m_active = true;
	g_graphicsEngine->addGfxEntity(m_gfxText);
}

void MenuItem::run() {
	std::string textToDraw = g_stringManager->getText(m_text);
	if (m_value != "")
		textToDraw += " " + g_stringManager->getText(m_value);
	if (m_selected)
		textToDraw = "- " + textToDraw + " -";
	
	m_gfxText->setPos(vmake(m_pos.x - (textToDraw.length() / 2.0f * 16), m_pos.y));
	m_gfxText->setText(textToDraw.c_str());

	//FONT_DrawString(vmake(m_pos.x - (textToDraw.length() / 2.0f * 16), m_pos.y), textToDraw.c_str());
}

void MenuItem::activate() {
	m_active = true;
	m_gfxText->activate();
}

void MenuItem::deactivate() {
	m_active = false;
	m_gfxText->deactivate();
}