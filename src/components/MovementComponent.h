#pragma once

#include "components/Component.h"

class CMovementComponent : public CComponent {
public:
	CMovementComponent(CEntity* _pOwner, const vec2& _v2Direction, float _fSpeed, bool _bHasBounce)
	: CComponent(_pOwner)
	, m_v2Direction(_v2Direction)
	, m_fSpeed(_fSpeed)
	, m_bHasBounce(_bHasBounce)
	{}

	// CComponent
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(TMessage* _pMessage) override;

private:
	vec2  m_v2Direction;
	float m_fSpeed;
	bool  m_bHasBounce;
};
