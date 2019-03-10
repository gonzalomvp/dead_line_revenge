#include "../common/stdafx.h"
#include "label.h"

#include "../engine/graphics_engine.h"
#include "../gui/string_manager.h"

//=============================================================================
// Label class
//=============================================================================
Label::~Label() {
	g_pGraphicsEngine->removeGfxEntity(m_labelText);
	DELETE(m_labelText);
}

void Label::init(const std::string& text) {
	m_text = text;
	m_labelText = NEW(Text, m_text, vmake(m_v2Pos.x - (m_text.length() / 2.0f * 16), m_v2Pos.y - 6), 1);
	g_pGraphicsEngine->addGfxEntity(m_labelText);
}

void Label::activate() {
	CControl::activate();

	m_labelText->activate();
	m_labelText->setPos(vmake(m_v2Pos.x - (m_text.length() / 2.0f * 16), m_v2Pos.y - 6));
}

void Label::deactivate() {
	CControl::deactivate();

	m_labelText->deactivate();
}

void Label::run(float _fDeltaTime) {
	if (m_bIsActive) {
		CControl::run(_fDeltaTime);

		std::string textToDraw = g_pStringManager->getText(m_text);
		m_labelText->setText(textToDraw.c_str());
		vec2 currentPos = m_labelText->getPos();
		currentPos.x = m_v2Pos.x - (textToDraw.length() / 2.0f * 16);
		m_labelText->setPos(currentPos);
	}
}
