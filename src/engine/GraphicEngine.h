#pragma once

#include <map>

class CGraphicEntity;

class CGraphicEngine {
public:
	CGraphicEngine();
	~CGraphicEngine();

	GLuint getTexture     (const std::string& _sFileName);
	void   addGfxEntity   (CGraphicEntity* _pGfxEntity);
	void   removeGfxEntity(const CGraphicEntity* _pGfxEntity);
	void   render         ();

private:
	std::vector<CGraphicEntity*>  m_vGfxEntities;
	std::map<std::string, GLuint> m_mTextures;
};
