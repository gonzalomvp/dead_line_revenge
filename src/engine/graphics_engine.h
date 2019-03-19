#pragma once

#include <map>

class CGfxEntity;

class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();

	GLuint getTexture     (const std::string& _sFileName);
	void   addGfxEntity   (CGfxEntity* _pGfxEntity);
	void   removeGfxEntity(const CGfxEntity* _pGfxEntity);
	void   render         ();

private:
	std::vector<CGfxEntity*>      m_vGfxEntities;
	std::map<std::string, GLuint> m_mTextures;
};
