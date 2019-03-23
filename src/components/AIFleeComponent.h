#pragma once

#include "components/component.h"

class CAIFleeComponent : public Component {
public:
	CAIFleeComponent(Entity* _pOwner, float _fSpeed, float _fRange) : Component(_pOwner), m_fSpeed(_fSpeed), m_fRange(_fRange) {}

	// CComponent
	virtual void run(float _fDeltaTime)             override;

private:
	static vec2 calculatIntersectionWithWorldLimits(const vec2& _v2Position, float _fAngle);

	float m_fSpeed;
	float m_fRange;
};