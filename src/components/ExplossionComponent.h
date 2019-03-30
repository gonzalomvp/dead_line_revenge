#pragma once

#include "components/Component.h"

class CExplossionComponent : public CComponent {
public:
	CExplossionComponent(Entity* _pOwner, vec2 _v2InitSize, vec2 _v2EndSize, float _fExplossionTime)
	: CComponent(_pOwner)
	, m_v2InitSize(_v2InitSize)
	, m_v2EndSize(_v2EndSize)
	, m_fExplossionTime(_fExplossionTime)
	, m_fExplossionTimer(_fExplossionTime)
	{}

	// CComponent
	virtual void run(float _fDeltaTime) override;

private:
	vec2  m_v2InitSize;
	vec2  m_v2EndSize;
	float m_fExplossionTime;
	float m_fExplossionTimer;
};