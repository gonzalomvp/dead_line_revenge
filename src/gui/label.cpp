#include "../common/stdafx.h"
#include "label.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"

//=============================================================================
// Label class
//=============================================================================
Label::Label(const std::string& name, const vec2& pos, const vec2& size, const std::string& text) : Control(name, pos, size, false) {
	m_text = text;
	m_gfxText = new Text(m_text, vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6), 1);
	g_graphicsEngine->addGfxEntity(m_gfxText);
	deactivate();
}

void Label::activate() {
	m_gfxText->activate();
	m_gfxText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6));
}

void Label::deactivate() {
	m_gfxText->deactivate();
}

void Label::run() {
	std::string textToDraw = g_stringManager->getText(m_text);
	m_gfxText->setText(textToDraw.c_str());
	vec2 currentPos = m_gfxText->getPos();
	currentPos.x = m_pos.x - (textToDraw.length() / 2.0f * 16);
	m_gfxText->setPos(currentPos);
}
