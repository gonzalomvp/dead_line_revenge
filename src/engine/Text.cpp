#include "common/stdafx.h"
#include "Text.h"

#include "gui/StringManager.h"

void CText::render() {
	ASSERT(g_pStringManager);
	std::string textToDraw = g_pStringManager->getText(m_sText);
	FONT_DrawString(m_v2Pos, textToDraw.c_str());
}