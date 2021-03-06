#include "common/stdafx.h"
#include "GraphicEngine.h"

#include "engine/GraphicEntity.h"

#include <algorithm>

CGraphicEngine::CGraphicEngine() {
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

CGraphicEngine::~CGraphicEngine() {
	for (auto itTexture = m_mTextures.begin(); itTexture != m_mTextures.end(); ++itTexture) {
		CORE_UnloadPNG(itTexture->second);
	}
	FONT_End();
}

GLuint CGraphicEngine::getTexture(const std::string& _sFileName) {
	GLuint uTextureId = 0;
	if (m_mTextures.count(_sFileName)) {
		uTextureId = m_mTextures[_sFileName];
	}
	else {
		uTextureId = CORE_LoadPNG(_sFileName.c_str(), false);
		m_mTextures[_sFileName] = uTextureId;
	}
	ASSERT(uTextureId != 0, "Texture %s not found", _sFileName.c_str());
	return uTextureId;
}

void CGraphicEngine::addGfxEntity(CGraphicEntity* _pGfxEntity) {
	m_vGfxEntities.push_back(_pGfxEntity);
}

void CGraphicEngine::removeGfxEntity(const CGraphicEntity* _pGfxEntity) {
	m_vGfxEntities.erase(
		std::remove(m_vGfxEntities.begin(), m_vGfxEntities.end(), _pGfxEntity),
		m_vGfxEntities.end()
	);
}

void CGraphicEngine::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	//sort by priority
	std::sort(m_vGfxEntities.begin(),
		m_vGfxEntities.end(),
		[](const CGraphicEntity* lhs, const CGraphicEntity* rhs) {
		return lhs->getPriority() > rhs->getPriority();
	});

	for (size_t i = 0; i < m_vGfxEntities.size(); ++i) {
		if (m_vGfxEntities[i]->isActive()) {
			m_vGfxEntities[i]->render();
		}
	}
	SYS_Show();
}

