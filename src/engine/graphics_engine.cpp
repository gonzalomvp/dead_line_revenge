#include "../common/stdafx.h"
#include "graphics_engine.h"

#include <algorithm>

void Sprite::render() {
	CORE_RenderCenteredSprite(m_pos, m_size, m_texture, m_alpha);
}

void Text::render() {
	FONT_DrawString(m_pos, m_text.c_str());
}

GraphicsEngine::GraphicsEngine() {
	FONT_Init();

	// Set up rendering
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Sets up clipping
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

void GraphicsEngine::addGfxEntity(GfxEntity* gfxEntity) {
	m_gfxEntities.push_back(gfxEntity);
}

void GraphicsEngine::removeGfxEntity(const GfxEntity* gfxEntity) {
	for (auto itGfxEntity= m_gfxEntities.begin(); itGfxEntity != m_gfxEntities.end(); ++itGfxEntity) {
		if (*itGfxEntity == gfxEntity) {
			m_gfxEntities.erase(itGfxEntity);
			break;
		}
	}
}

void GraphicsEngine::removeAllGfxEntities() {
	m_gfxEntities.clear();
}

GLuint GraphicsEngine::getTexture(const char* fileName) {
	GLuint res;
	if (m_textures[fileName]) {
		res = m_textures[fileName];
	}
	else {
		res = CORE_LoadPNG(fileName, false);
		m_textures[fileName] = res;
	}
	return res;
}

void GraphicsEngine::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	//sort by priority
	std::sort(m_gfxEntities.begin(),
		m_gfxEntities.end(),
		[](const GfxEntity* lhs, const GfxEntity* rhs)
	{
		return lhs->getPriority() > rhs->getPriority();
	});

	for (size_t i = 0; i < m_gfxEntities.size(); i++) {
		m_gfxEntities[i]->render();
	}
	SYS_Show();
}