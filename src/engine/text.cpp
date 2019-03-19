#include "common/stdafx.h"
#include "text.h"

#include "gui/string_manager.h"

void CText::render() {
	ASSERT(g_pStringManager);
	std::string textToDraw = g_pStringManager->getText(m_sText);
	FONT_DrawString(m_v2Pos, textToDraw.c_str());
}