#pragma once

#include "components/Component.h"
#include "entities/Entity.h"

class CWeaponComponent : public CComponent, public CEntity::IListener {
public:
	enum EType {

#define REG_WEAPON(val, name) \
		E##val,
#include "REG_WEAPONS.h"
#undef REG_WEAPON

		EInvalid,
	};
	static const int NUM_WEAPON_TYPES = EInvalid;
	static CWeaponComponent::EType getWeaponTypeByName(const std::string& name);

	CWeaponComponent(CEntity* _pOwner, EType _eType, vec2 _v2AimDir = vmake(0.0f, 0.0f))
	: CComponent(_pOwner)
	, m_eType(_eType)
	, m_v2AimDir(_v2AimDir)
	, m_pRemoteBullet(nullptr)
	, m_iMaxBullets(0)
	, m_iCurrentBullets(0)
	, m_uBulletsPerShot(0)
	, m_fFireRate(0.0f)
	, m_fReloadTime(0.0f)
	, m_bIsAutomatic(false)
	, m_sSoundFile("")
	, m_bIsFiring(false)
	, m_fFireTimer(0.0f)
	, m_fReloadTimer(0.0f)
	{}

	~CWeaponComponent();

	void equipWeapon(EType _eType);

	// CComponent
	virtual void init()                             override;
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(TMessage* _pMessage) override;

	// CEntity::IListener
	virtual void onEntityDestroyed(CEntity* _pEntity) override;

private:
	EType       m_eType;
	vec2        m_v2AimDir;
	CEntity*    m_pRemoteBullet;
	int         m_iMaxBullets;
	int         m_iCurrentBullets;
	uint16_t    m_uBulletsPerShot;
	float       m_fFireRate;
	float       m_fReloadTime;
	bool        m_bIsAutomatic;
	std::string m_sSoundFile;
	bool        m_bIsFiring;

	//Timers
	float       m_fFireTimer;
	float       m_fReloadTimer;

	struct TWeaponInfo {
		CWeaponComponent::EType eType;
		std::string             sName;
	};
	static TWeaponInfo s_aWeaponInfo[];
};
