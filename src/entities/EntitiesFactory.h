#pragma once

#include "components/WeaponComponent.h"
#include "entities/Entity.h"

#include <map>

class CEntitiesFactory {
public:
	struct TWeaponDef {
		CWeaponComponent::EType eType;
		float                   fFireRate;
		float                   fReloadTime;
		int                     iMaxBullets;
		float                   fBulletSpeed;
		int                     iBulletDamage;
		int                     iBulletLife;
		float                   fBulletRange;
		vec2                    v2BulletSize;
		uint16_t                uBulletsPerShot;
		bool                    bIsAutomatic;
		bool                    bIsExplossive;
		bool                    bIsBouncy;
		std::string             sImageFile;
		std::string             sSoundFile;
	};

	struct TEnemyDef {
		CEntity::EType           eType;
		int                      iLife;
		float                    fInvencibleTime;
		float                    fSpeed;
		int                      iCollisionDamage;
		CWeaponComponent::EType  eWeapon;
		vec2                     v2Size;
		std::string              sImageFile;
		std::string              sBTFile;
	};

	CEntitiesFactory()  {}
	~CEntitiesFactory() {}

	bool init(const char* _sConfigFile);

	CEntity* createPlayer(const vec2& _v2Pos);
	CEntity* createBullet(const CWeaponComponent::EType& _eWeaponType, const vec2& _v2Pos, const vec2& _v2Direction, const CEntity::EType& _eOwnerType);
	CEntity* createExplossion(const vec2& _v2Pos, const CWeaponComponent::EType& _eWeaponType = CWeaponComponent::EInvalid);
	CEntity* createEnemy(const vec2& _v2Pos, const CEntity::EType& _eEnemyType, const std::string& _sBTFile = "", const vec2& _v2MoveDir = vmake(0.0f, 0.0f), const vec2& _vAimDir = vmake(0.0f, 0.0f));
	CEntity* createWeaponPickup();

	TWeaponDef getWeaponDef(const CWeaponComponent::EType& _eWeaponType) { return m_mWeaponDef[_eWeaponType]; }
	TEnemyDef  getEnemyDef (const CEntity::EType& _eEnemyType)           { return m_mEnemyDef[_eEnemyType];   }
	
	std::map<CWeaponComponent::EType, TWeaponDef> m_mWeaponDef;
	std::map<CEntity::EType, TEnemyDef>           m_mEnemyDef;
	std::vector<CWeaponComponent::EType>          m_vWeaponPickups;
};
