#pragma once

#include "engine/graphics_entitiy.h"

class CSprite : public CGfxEntity {
public:
	CSprite(GLuint _uTexture, vec2 _v2Pos, vec2 _v2Size, float _fAngle, float _fAlpha, int _iPriority) : CGfxEntity(_v2Pos, _iPriority), m_uTexture(_uTexture), m_v2Size(_v2Size), m_fAngle(_fAngle), m_fAlpha(_fAlpha) {}

	GLuint getTexture() const           { return m_uTexture;      }
	void   setTexture(GLuint _uTexture) { m_uTexture = _uTexture; }
	vec2   getSize   () const           { return m_v2Size;        }
	void   setSize   (vec2 _v2Size)     { m_v2Size = _v2Size;     }
	float  getAngle  () const           { return m_fAngle;        }
	void   setAngle  (float _fAngle)    { m_fAngle = _fAngle;     }
	float  getAlpha  () const           { return m_fAlpha;        }
	void   setAlpha  (float _fAlpha)    { m_fAlpha = _fAlpha;     }

	// CGfxEntity
	virtual void render() override;

private:
	GLuint m_uTexture;
	vec2   m_v2Size;
	float  m_fAngle;
	float  m_fAlpha;
};
