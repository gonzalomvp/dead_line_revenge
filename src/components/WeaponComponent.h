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
	, m_eType(_eType)
	, m_pRemoteBullet(nullptr)
	, m_v2AimDirection(vmake(0.0f, 0.0f))
	, m_iMaxBullets(0)
	, m_iCurrentBullets(0)
	, m_uBulletsPerShot(0)
	, m_iFireRate(0)
	, m_iReloadTime(0)
	, m_bIsAutomatic(false)
	, m_sSoundFile("")
	, m_bIsFiring(false)
	, m_iFireTimer(0)
	, m_iReloadTimer(0)
	{}

	void equipWeapon(EType _eType);

	// CComponent
	virtual void init()                             override;
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

private:
	EType       m_eType;
	Entity*     m_pRemoteBullet;
	vec2        m_v2AimDirection;
	int         m_iMaxBullets;
	int         m_iCurrentBullets;
	uint16_t    m_uBulletsPerShot;
	int         m_iFireRate;
	int         m_iReloadTime;
	bool        m_bIsAutomatic;
	std::string m_sSoundFile;
	bool        m_bIsFiring;

	//Timers
	int m_iFireTimer;
	int m_iReloadTimer;
};
