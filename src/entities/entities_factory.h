#pragma once

#include "entities/components/component.h"
#include "entities/entity.h"

class CEntitiesFactory {
public:
	struct TEnemyDef {
		Entity::TType            eType;
		int                      iLife;
		float                    fSpeed;
		int                      iCollisionDamage;
		ComponentWeapon::TType   eWeapon;
		vec2                     v2Size;
		std::string              sImageFile;
	};

	//CEntitiesFactory();
	//~CEntitiesFactory();

	bool init(const char* _sConfigFile);

	Entity* createPlayer(vec2 _v2Pos);
	Entity* createBullet(vec2 _v2Pos, vec2 _v2Size, vec2 _v2Direction, float _fSpeed, int _iDamage, int _iLife, int _iRange, bool _bIsExplossive, bool _bIsBouncy, Entity::TType _eEntityType, const char* _psTexture);
	Entity* createExplossion(vec2 _v2Pos, vec2 _v2Size, vec2 _v2SizeIncrement, int _iDuration, Entity::TType _eEntityType);
	Entity* createEnemy(vec2 _v2Pos, Entity::TType _tEnemyType, Entity* _pPlayer);
	Entity* createEnemy(vec2 _v2Pos, Entity::TType _tEnemyType, vec2 _v2MoveDir, std::vector<vec2> _vAimDirections, bool _bIshuffleAim);
	Entity* createRandomEnemy(vec2 _v2Pos);
	Entity* createWeaponPickup();
	Entity* createHUDMessage(const std::string& _sMessage, vec2 _v2Pos, int _iDisplayTime);

	TEnemyDef getEnemyDef(const Entity::TType& _tEnemyType) { return m_mEnemyDef[_tEnemyType]; }
	ComponentWeapon::TWeaponData getWeaponDef(const ComponentWeapon::TType& _tWeaponType) { return m_mWeaponDef[_tWeaponType]; }

	std::map<Entity::TType, TEnemyDef> m_mEnemyDef;
	std::map<ComponentWeapon::TType, ComponentWeapon::TWeaponData> m_mWeaponDef;
	std::vector<ComponentWeapon::TType> m_vWeaponPickups;
};