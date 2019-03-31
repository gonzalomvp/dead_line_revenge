#pragma once

class CGraphicEntity {
public:
	CGraphicEntity(vec2 _v2Pos, int _iPriority, bool _bIsActive = true) : m_v2Pos(_v2Pos), m_iPriority(_iPriority), m_bIsActive(_bIsActive) {}
	~CGraphicEntity();

	virtual void activate  ()     { m_bIsActive = true;  }
	virtual void deactivate()     { m_bIsActive = false; }
	virtual void render    ()     = 0;

	vec2 getPos() const      { return m_v2Pos;   }
	void setPos(vec2 _v2Pos) { m_v2Pos = _v2Pos; }

	int  getPriority() const         { return m_iPriority;       }
	void setPriority(int _iPriority) { m_iPriority = _iPriority; }

	bool isActive() const { return m_bIsActive; }

protected:
	vec2   m_v2Pos;
	int    m_iPriority;
	bool   m_bIsActive;
};