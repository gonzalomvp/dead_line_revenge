#pragma once

#include "components/component.h"

class CWeaponComponent : public Component {
public:
	enum EType {

#define REG_WEAPON(val, name) \
		E##val,
#include "REG_WEAPONS.h"
#undef REG_WEAPON

		EInvalid,
	};
	static const int NUM_WEAPON_TYPES = EInvalid;

	CWeaponComponent(Entity* owner, EType _eType)
	: Component(owner)
	, m_eType(m_eType)
	, m_pRemoteBullet(nullptr)
	, m_v2AimDirection(vmake(0.0f, 0.0f))
	, m_iCurrentBullets(0)
	, m_bIsFiring(false)
	, m_iFireTimer(0)
	, m_iReloadTimer(0)
	{}

	// CComponent
	virtual void init()                             override;
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

private:
	EType                        m_eType;
	Entity*                      m_pRemoteBullet;
	vec2                         m_v2AimDirection;
	int                          m_iCurrentBullets;
	bool                         m_bIsFiring;

	//Timers
	int m_iFireTimer;
	int m_iReloadTimer;
};
