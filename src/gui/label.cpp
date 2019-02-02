#include "../common/stdafx.h"
#include "label.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"

//=============================================================================
// Label class
//=============================================================================
Label::~Label() {
	g_graphicsEngine->removeGfxEntity(m_labelText);
	DELETE(m_labelText);
}

void Label::init(const std::string& text) {
	m_text = text;
	m_labelText = new Text(m_text, vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6), 1);
	g_graphicsEngine->addGfxEntity(m_labelText);
}

void Label::activate() {
	Control::activate();

	m_labelText->activate();
	m_labelText->setPos(vmake(m_pos.x - (m_text.length() / 2.0f * 16), m_pos.y - 6));
}

void Label::deactivate() {
	Control::deactivate();

	m_labelText->deactivate();
}

void Label::run() {
	if (m_isActive) {
		Control::run();

		std::string textToDraw = g_stringManager->getText(m_text);
		m_labelText->setText(textToDraw.c_str());
		vec2 currentPos = m_labelText->getPos();
		currentPos.x = m_pos.x - (textToDraw.length() / 2.0f * 16);
		m_labelText->setPos(currentPos);
	}
}
