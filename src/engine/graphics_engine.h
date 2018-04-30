#pragma once

#include <map>

class GfxEntity;

//=============================================================================
// GraphicsEngine class
//=============================================================================

class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();

	GLuint getTexture     (const char* fileName);
	void   addGfxEntity   (GfxEntity* gfxEntity);
	void   removeGfxEntity(const GfxEntity* gfxEntity);
	void   render         ();
private:
	std::vector<GfxEntity*>       m_gfxEntities;
	std::map<std::string, GLuint> m_textures;
};

//=============================================================================
// GfxEntity class
//=============================================================================
class GfxEntity {
public:
	GfxEntity(vec2 pos, int priority) : m_pos(pos), m_priority(priority), m_isActive(true) {}
	~GfxEntity();

	void activate  ()     { m_isActive = true;  }
	void deactivate()     { m_isActive = false; }
	bool isActive() const { return m_isActive;  }

	vec2 getPos() const   { return m_pos; }
	void setPos(vec2 pos) { m_pos = pos;  }

	int  getPriority() const       { return m_priority;     }
	void setPriority(int priority) { m_priority = priority; }

	virtual void render() = 0;
protected:
	vec2   m_pos;
	int    m_priority;
	bool   m_isActive;
};

//=============================================================================
// Sprite class
//=============================================================================
class Sprite : public GfxEntity {
public:
	Sprite(GLuint texture, vec2 pos, vec2 size, float angle, float alpha, int priority) : GfxEntity(pos, priority), m_texture(texture), m_size(size), m_angle(angle), m_alpha(alpha) {}

	GLuint getTexture() const         { return m_texture;    }
	void   setTexture(GLuint texture) { m_texture = texture; }

	vec2 getSize() const    { return m_size; }
	void setSize(vec2 size) { m_size = size; }

	float getAngle() const      { return m_angle;  }
	void  setAngle(float angle) { m_angle = angle; }

	float getAlpha() const      { return m_alpha;  }
	void  setAlpha(float alpha) { m_alpha = alpha; }

	virtual void render();
private:
	GLuint m_texture;
	vec2   m_size;
	float  m_angle;
	float  m_alpha;
};

//=============================================================================
// Text class
//=============================================================================
class Text : public GfxEntity {
public:
	Text(std::string text, vec2 pos, int priority) : GfxEntity(pos, priority), m_text(text) {}

	std::string getText() const                  { return m_text; }
	void        setText(const std::string& text) { m_text = text; }
	
	virtual void render();
private:
	std::string m_text;
};