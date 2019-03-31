#pragma once

#include "components/Component.h"

class CLifeComponent : public CComponent {
public:
	CLifeComponent(CEntity* _pOwner, int _iLife, float _fInvencibleTime = 0.0, float _fTimeToLive = -1.0f)
	: CComponent(_pOwner)
	, m_iLife(_iLife)
	, m_fInvencibleTime(_fInvencibleTime)
	, m_fLifeTimer(_fTimeToLive)
	, m_fInvencibleTimer(0)
	{}

	// CComponent
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(TMessage* _pMessage) override;

private:
	void onDead();

	int   m_iLife;
	float m_fInvencibleTime;

	//Timers
	float m_fLifeTimer;
	float m_fInvencibleTimer;
};
