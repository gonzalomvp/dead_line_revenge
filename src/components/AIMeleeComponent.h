#pragma once

#include "components/component.h"

class CAIMeleeComponent : public Component {
public:
	CAIMeleeComponent(Entity* _pOwner, float _fSpeed, float _fMaxDistance);

	// CComponent
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

private:
	float       m_fSpeed;
	float       m_fMaxDistance;
	vec2        m_v2Offset;
};