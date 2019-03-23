#pragma once

#include "components/AIComponent.h"

class CAIMeleeComponent : public CAIComponent {
public:
	CAIMeleeComponent(Entity* _pOwner, float _fSpeed, float _fMaxDistance);

	// CComponent
	virtual void run(float _fDeltaTime) override;

private:
	float m_fSpeed;
	float m_fMaxDistance;
	vec2  m_v2Offset;
};