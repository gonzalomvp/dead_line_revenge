#include "../common/stdafx.h"
#include "graphics_engine.h"

#include "../gui/string_manager.h"
#include <algorithm>

//=============================================================================
// GraphicsEngine class
//=============================================================================
GraphicsEngine::GraphicsEngine() {
	FONT_Init();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClearColor(0.0f, 0.1f, 0.3f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ShowCursor(false);
}

GraphicsEngine::~GraphicsEngine() {
	for (auto itTextures = m_textures.begin(); itTextures != m_textures.end(); ++itTextures) {
		CORE_UnloadPNG(itTextures->second);
	}
	FONT_End();
}

GLuint GraphicsEngine::getTexture(const char* fileName) {
	GLuint textureId = m_textures[fileName];
	if (!textureId) {
		textureId = CORE_LoadPNG(fileName, false);
		m_textures[fileName] = textureId;
	}
	return textureId;
}

void GraphicsEngine::addGfxEntity(GfxEntity* gfxEntity) {
	m_gfxEntities.push_back(gfxEntity);
}

void GraphicsEngine::removeGfxEntity(const GfxEntity* gfxEntity) {
	for (auto itGfxEntity = m_gfxEntities.begin(); itGfxEntity != m_gfxEntities.end(); ++itGfxEntity) {
		if (*itGfxEntity == gfxEntity) {
			m_gfxEntities.erase(itGfxEntity);
			break;
		}
	}
}

void GraphicsEngine::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	//sort by priority
	std::sort(m_gfxEntities.begin(),
		m_gfxEntities.end(),
		[](const GfxEntity* lhs, const GfxEntity* rhs) {
		return lhs->getPriority() > rhs->getPriority();
	});

	for (size_t i = 0; i < m_gfxEntities.size(); i++) {
		if (m_gfxEntities[i]->isActive()) {
			m_gfxEntities[i]->render();
		}
	}
	SYS_Show();
}

//=============================================================================
// GfxEntity class
//=============================================================================
GfxEntity::~GfxEntity() {
	if (g_graphicsEngine) {
		g_graphicsEngine->removeGfxEntity(this);
	}
}

//=============================================================================
// Sprite class
//=============================================================================
void Sprite::render() {
	CORE_RenderCenteredRotatedSprite(m_pos, m_size, DEG2RAD(m_angle), m_texture, rgbamake(255, 255, 255, static_cast<int>(255 * m_alpha)));
}

//=============================================================================
// Text class
//=============================================================================
void Text::render() {
	std::string textToDraw = g_stringManager->getText(m_text);
	FONT_DrawString(m_pos, textToDraw.c_str());
}
