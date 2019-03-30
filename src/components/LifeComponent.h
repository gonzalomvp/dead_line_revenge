#pragma once

#include "components/component.h"

class CLifeComponent : public Component {
public:
	CLifeComponent(Entity* _pOwner, int _iLife, int _iInvencibleTime = -1, float _fTimeToLive = -1.0f)
	: Component(_pOwner)
	, m_iLife(_iLife)
	, m_iInvencibleTime(_iInvencibleTime)
	, m_fLifeTimer(_fTimeToLive)
	, m_iInvencibleTimer(0)
	{}

	// CComponent
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

private:
	void onDead();

	int m_iLife;
	int m_iInvencibleTime;

	//Timers
	float m_fLifeTimer;
	int m_iInvencibleTimer;
};
