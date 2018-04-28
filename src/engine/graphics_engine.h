#pragma once

#include <map>

class Sprite;
class GfxEntity;

//=============================================================================
// GraphicsEngine class
class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();

	void   addGfxEntity       (GfxEntity* gfxEntity);
	void   removeGfxEntity(const GfxEntity* gfxEntity);
	void   removeAllGfxEntities();
	GLuint getTexture      (const char* fileName);
	void   render          ();

private:
	std::vector<GfxEntity*>          m_gfxEntities;
	std::map<std::string, GLuint>    m_textures;
};

//incluir un gestor de texturas

//=============================================================================
// GraphicsEntity class
class GfxEntity {
public:
	GfxEntity(vec2 pos, int priority) : m_pos(pos), m_priority(priority), m_isActive(true) {}

	void activate() { if (!m_isActive) { m_isActive = true; g_graphicsEngine->addGfxEntity(this); } }
	void deactivate() { if (m_isActive) { m_isActive = false; g_graphicsEngine->removeGfxEntity(this); } }

	void setPos(vec2 pos) { m_pos = pos; }
	vec2 getPos() { return m_pos; }

	int getPriority() const { return m_priority; }
	virtual void render() = 0;

protected:
	vec2   m_pos;
	int    m_priority;
	bool   m_isActive;
};

//=============================================================================
// Sprite class
class Sprite : public GfxEntity {
public:
	Sprite(GLuint texture, int priority, float alpha = 1.0f, float angle = 0.0f, vec2 pos = vmake(0, 0)) : GfxEntity(pos, priority), m_alpha(alpha), m_texture(texture), m_size(vmake(0, 0)), m_angle(angle) {}

	virtual void setSize(vec2 size) { m_size = size; }
	virtual void setAngle(float angle) { m_angle = angle; }
	vec2 getSize() { return m_size; }
	virtual void setTexture(GLuint texture) { m_texture = texture;  }
	virtual void render();

private:
	vec2   m_size;
	GLuint m_texture;
	float  m_alpha;
	float  m_angle;
};

//=============================================================================
// Text class
class Text : public GfxEntity {
public:
	Text(std::string text, int priority, vec2 pos = vmake(0, 0)) : GfxEntity(pos, priority), m_text(text) {}

	void setText(std::string text) { m_text = text; }
	std::string getText() { return m_text; }
	virtual void render();

private:
	std::string m_text;
};