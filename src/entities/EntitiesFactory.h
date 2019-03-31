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

	//CEntitiesFactory();
	//~CEntitiesFactory();

	static CWeaponComponent::EType getWeaponTypeByName(const std::string& name);
	static CEntity::EType          getEntityTypeByName(const std::string& name);
	
	bool init(const char* _sConfigFile);

	CEntity* createPlayer(vec2 _v2Pos);
	CEntity* createBullet(CWeaponComponent::EType _eWeaponType, vec2 _v2Pos, vec2 _v2Direction, CEntity::EType _eOwnerType);
	CEntity* createExplossion(vec2 _v2Pos, CWeaponComponent::EType _eWeaponType = CWeaponComponent::EInvalid);
	CEntity* createEnemy(vec2 _v2Pos, CEntity::EType _tEnemyType, const std::string& _sBTFile = "", vec2 _v2MoveDir = vmake(0.0f, 0.0f), vec2 _vAimDir = vmake(0.0f, 0.0f));
	CEntity* createWeaponPickup();

	TWeaponDef getWeaponDef(const CWeaponComponent::EType& _tWeaponType) { return m_mWeaponDef[_tWeaponType]; }
	TEnemyDef  getEnemyDef(const CEntity::EType& _tEnemyType)             { return m_mEnemyDef[_tEnemyType];   }
	

	std::map<CWeaponComponent::EType, TWeaponDef> m_mWeaponDef;
	std::map<CEntity::EType, TEnemyDef>            m_mEnemyDef;
	std::vector<CWeaponComponent::EType>          m_vWeaponPickups;

private:
	struct TWeaponInfo {
		CWeaponComponent::EType eType;
		std::string sName;
	};
	static TWeaponInfo s_aWeaponInfo[];

	struct TEntityInfo {
		CEntity::EType eType;
		std::string sName;
	};
	static TEntityInfo s_aEntityInfo[];
};