#pragma once

#include "components/WeaponComponent.h"
#include "entities/entity.h"

#include <map>

class CEntitiesFactory {
public:
	struct TEnemyDef {
		Entity::EType            eType;
		int                      iLife;
		int                      iInvencibleTime;
		float                    fSpeed;
		int                      iCollisionDamage;
		CWeaponComponent::EType  eWeapon;
		vec2                     v2Size;
		std::string              sImageFile;
	};

	//CEntitiesFactory();
	//~CEntitiesFactory();

	static Entity::EType getEntityTypeByName(const std::string& name);

	bool init(const char* _sConfigFile);

	Entity* createPlayer(vec2 _v2Pos);
	Entity* createBullet(vec2 _v2Pos, vec2 _v2Size, vec2 _v2Direction, float _fSpeed, int _iDamage, int _iLife, int _iRange, bool _bIsExplossive, bool _bIsBouncy, Entity::EType _eEntityType, const char* _psTexture);
	Entity* createExplossion(vec2 _v2Pos, vec2 _v2Size, vec2 _v2SizeIncrement, int _iDuration, Entity::EType _eEntityType);
	Entity* createEnemy(vec2 _v2Pos, Entity::EType _tEnemyType, Entity* _pPlayer);
	Entity* createEnemy(vec2 _v2Pos, Entity::EType _tEnemyType, vec2 _v2MoveDir, std::vector<vec2> _vAimDirections, bool _bIshuffleAim);
	Entity* createWeaponPickup();

	TEnemyDef getEnemyDef(const Entity::EType& _tEnemyType) { return m_mEnemyDef[_tEnemyType]; }
	CWeaponComponent::TWeaponData getWeaponDef(const CWeaponComponent::EType& _tWeaponType) { return m_mWeaponDef[_tWeaponType]; }

	std::map<Entity::EType, TEnemyDef> m_mEnemyDef;
	std::map<CWeaponComponent::EType, CWeaponComponent::TWeaponData> m_mWeaponDef;
	std::vector<CWeaponComponent::EType> m_vWeaponPickups;

private:
	struct TEntityInfo {
		Entity::EType eType;
		std::string sName;
	};

	static TEntityInfo s_aEntityInfo[];
};