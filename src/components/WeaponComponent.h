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

	struct TWeaponData {
		EType       eType;
		int         iFireRate;
		int         iReloadTime;
		int         iCapacity;
		float       fBulletSpeed;
		int         iBulletDamage;
		int         iBulletLife;
		int         iBulletRange;
		uint16_t    uNumBullets;
		bool        bIsAutomatic;
		bool        bIsExplossive;
		bool        bIsBouncy;
		std::string sSoundFile;
	};

	static EType getWeaponTypeByName(const std::string& name);

	CWeaponComponent(Entity* owner, TWeaponData weaponData)
	: Component(owner)
	, m_mWeaponData(weaponData)
	, m_remoteBullet(nullptr)
	, m_aimDirection(vmake(0.0f, 0.0f))
	, m_isFiring(false)
	, m_currentBullets(m_mWeaponData.iCapacity)
	, m_fireTimer(m_mWeaponData.iFireRate)
	, m_reloadTimer(m_mWeaponData.iReloadTime)
	{}

	// CComponent
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

private:
	TWeaponData m_mWeaponData;
	Entity*     m_remoteBullet;
	vec2        m_aimDirection;
	int         m_currentBullets;
	bool        m_isFiring;

	//Timers
	int m_fireTimer;
	int m_reloadTimer;

	struct TWeaponInfo
	{
		EType eType;
		std::string sName;
	};
	static TWeaponInfo s_aWeaponInfo[];
};
