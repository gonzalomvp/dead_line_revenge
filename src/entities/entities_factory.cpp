#include "common/stdafx.h"
#include "entities_factory.h"

#include "engine/sound_engine.h"

#include "components/AIFireComponent.h"
#include "components/AIFleeComponent.h"
#include "components/AIMeleeComponent.h"
#include "components/ColliderComponent.h"
#include "components/ExplossiveComponent.h"
#include "components/HUDComponent.h"
#include "components/LifeComponent.h"
#include "components/MovementComponent.h"
#include "components/PlayerControllerComponent.h"
#include "components/PointsComponent.h"
#include "components/RenderableComponent.h"
#include "components/TransformComponent.h"
#include "components/WeaponComponent.h"
#include "components/WeaponPickupComponent.h"
#include "components/bossIAComponent.h"

#include "scenes/world.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

CEntitiesFactory::TWeaponInfo CEntitiesFactory::s_aWeaponInfo[] = {
#define REG_WEAPON(val, name) \
	{CWeaponComponent::E##val, name},
#include "REG_WEAPONS.h"
#undef REG_WEAPON
};

CEntitiesFactory::TEntityInfo CEntitiesFactory::s_aEntityInfo[] = {
#define REG_ENTITY(val, name) \
	{Entity::E##val, name},
#include "REG_ENTITIES.h"
#undef REG_ENTITY
};

CWeaponComponent::EType CEntitiesFactory::getWeaponTypeByName(const std::string& name) {
	CWeaponComponent::EType etype = CWeaponComponent::EInvalid;
	int i = 0;
	while ((etype == CWeaponComponent::EInvalid) && (i < CWeaponComponent::NUM_WEAPON_TYPES)) {
		if (name == s_aWeaponInfo[i].sName) {
			etype = s_aWeaponInfo[i].eType;
		}
		i++;
	}
	return etype;
}


Entity::EType CEntitiesFactory::getEntityTypeByName(const std::string& name) {
	Entity::EType etype = Entity::EInvalid;
	int i = 0;
	while ((etype == Entity::EInvalid) && (i < Entity::NUM_ENTITIES)) {
		if (name == s_aEntityInfo[i].sName) {
			etype = s_aEntityInfo[i].eType;
		}
		i++;
	}
	return etype;
}

bool CEntitiesFactory::init(const char* _sConfigFile) {
	FILE* file = fopen(_sConfigFile, "r");
	if (!file) return false;

	fseek(file, 0, SEEK_END);
	std::vector<char> fileData(ftell(file));
	fseek(file, 0, SEEK_SET);
	FileReadStream is(file, fileData.data(), fileData.size());
	Document doc;
	doc.ParseStream(is);

	// Load weapon definitions
	const Value& weapons = doc["weapons"];
	for (SizeType i = 0; i < weapons.Size(); i++) {
		TWeaponDef weapon;
		weapon.eType = getWeaponTypeByName(weapons[i]["name"].GetString());
		weapon.iFireRate = weapons[i]["fireRate"].GetInt();
		weapon.iReloadTime = weapons[i]["reloadTime"].GetInt();
		weapon.iMaxBullets = weapons[i]["maxBullets"].GetInt();
		weapon.fBulletSpeed = weapons[i]["bulletSpeed"].GetFloat();
		weapon.iBulletDamage = weapons[i]["bulletDamage"].GetInt();
		weapon.iBulletLife = weapons[i]["bulletLife"].GetInt();
		weapon.iBulletRange = weapons[i]["bulletRange"].GetInt();

		ASSERT(weapons[i].HasMember("bulletSize") && weapons[i]["bulletSize"].Size() == 2);
		weapon.v2BulletSize = vmake(weapons[i]["bulletSize"][0].GetFloat(), weapons[i]["bulletSize"][1].GetFloat());

		weapon.uBulletsPerShot = 1;
		if (weapons[i].HasMember("bulletsPerShot")) {
			weapon.uBulletsPerShot = weapons[i]["bulletsPerShot"].GetUint();
		}
		weapon.bIsAutomatic = weapons[i]["isAutomatic"].GetBool();
		weapon.bIsExplossive = weapons[i]["isExplossive"].GetBool();
		weapon.bIsBouncy = weapons[i]["isBouncy"].GetBool();

		ASSERT(weapons[i].HasMember("imageFile"));
		weapon.sImageFile = weapons[i]["imageFile"].GetString();

		weapon.sSoundFile = weapons[i]["soundFile"].GetString();
		m_mWeaponDef[weapon.eType] = weapon;
	}

	// Load enemy definitions
	const Value& enemies = doc["enemies"];
	for (SizeType i = 0; i < enemies.Size(); i++) {
		TEnemyDef enemy;
		enemy.eType = getEntityTypeByName(enemies[i]["type"].GetString());
		enemy.iLife = enemies[i]["life"].GetInt();
		enemy.iInvencibleTime = enemies[i]["invencibleTime"].GetInt();
		enemy.fSpeed = enemies[i]["speed"].GetFloat();
		enemy.iCollisionDamage = enemies[i]["collisionDamage"].GetInt();
		enemy.eWeapon = CWeaponComponent::EType::EInvalid;
		if (enemies[i].HasMember("weapon")) {
			enemy.eWeapon = getWeaponTypeByName(enemies[i]["weapon"].GetString());
		}
		enemy.v2Size = vmake(enemies[i]["size"][0].GetFloat(), enemies[i]["size"][1].GetFloat());
		enemy.sImageFile = enemies[i]["imageFile"].GetString();
		m_mEnemyDef[enemy.eType] = enemy;
	}

	// Load weapon pickups
	const Value& pickups = doc["pickups"];
	for (SizeType i = 0; i < pickups.Size(); i++) {
		m_vWeaponPickups.push_back(getWeaponTypeByName(pickups[i].GetString()));
	}

	fclose(file);

	return true;
}

Entity* CEntitiesFactory::createPlayer(vec2 _v2Pos) {
	Entity* player = NEW(Entity, Entity::EPLAYER);
	CTransformComponent* transform = NEW(CTransformComponent, player, _v2Pos, vmake(30, 25));
	transform->init();
	CRenderableComponent* renderable = NEW(CRenderableComponent, player, "data/player.png", 0.0f, 1.0f, 5, 10);
	renderable->init();
	CPlayerControllerComponent* playerControl = NEW(CPlayerControllerComponent, player);
	playerControl->init();
	CMovementComponent* movement = NEW(CMovementComponent, player, vmake(0.0f, 0.0f), g_pWorld->getPlayerSpeed(), false, false);
	movement->init();
	CWeaponComponent* weapon = NEW(CWeaponComponent, player, CWeaponComponent::EREVOLVER);
	weapon->init();
	CColliderComponent* collider = NEW(CColliderComponent, player, CColliderComponent::ERectCollider, -1, CColliderComponent::EPlayerCollider, CColliderComponent::EEnemyCollider | CColliderComponent::EEnemyWeaponCollider);
	collider->init();
	CLifeComponent* life = NEW(CLifeComponent, player, g_pWorld->getPlayerLife(), 0, 20);
	life->init();
	CHUDComponent* hudComponent = NEW(CHUDComponent, player);
	hudComponent->init();
	return player;
}

Entity* CEntitiesFactory::createBullet(CWeaponComponent::EType _eWeaponType, vec2 _v2Pos, vec2 _v2Direction, Entity::EType _eOwnerType) {
	Entity* bullet = NEW(Entity, Entity::EWEAPON);

	if (!m_mWeaponDef.count(_eWeaponType)) {
		ASSERT(false, "Weapon definition not found");
		return nullptr;
	}
	TWeaponDef weaponData = m_mWeaponDef[_eWeaponType];

	CTransformComponent* transform = NEW(CTransformComponent, bullet, _v2Pos, weaponData.v2BulletSize);
	transform->init();
	CRenderableComponent* renderable = NEW(CRenderableComponent, bullet, weaponData.sImageFile, vangle(_v2Direction), 1.0f, 5);
	renderable->init();
	CMovementComponent* movement = NEW(CMovementComponent, bullet, _v2Direction, weaponData.fBulletSpeed, true, weaponData.bIsBouncy);
	movement->init();

	// Depending on the type of bullet it has different collider setup
	switch (_eOwnerType) {
	case Entity::EPLAYER: {
		if (_eWeaponType == CWeaponComponent::EMINES) {
			CColliderComponent* collider = NEW(CColliderComponent, bullet, CColliderComponent::ECircleCollider, 0, CColliderComponent::ENoneCollider, CColliderComponent::EPlayerCollider | CColliderComponent::EEnemyCollider | CColliderComponent::EPlayerWeaponCollider | CColliderComponent::EEnemyWeaponCollider);
			collider->setActivationDelay(20);
			collider->init();
		}
		else {
			CColliderComponent* collider = NEW(CColliderComponent, bullet, CColliderComponent::ECircleCollider, weaponData.iBulletDamage, CColliderComponent::EPlayerWeaponCollider, CColliderComponent::EEnemyCollider | CColliderComponent::EBoundariesCollider);
			collider->init();
		}
		break;
	}
	default: {
		CColliderComponent* collider = NEW(CColliderComponent, bullet, CColliderComponent::ECircleCollider, weaponData.iBulletDamage, CColliderComponent::EEnemyWeaponCollider, CColliderComponent::EPlayerCollider | CColliderComponent::EBoundariesCollider);
		collider->init();
		break;
	}
	}
	if (weaponData.bIsExplossive) {
		CExplossiveComponent* explossive = NEW(CExplossiveComponent, bullet);
		explossive->init();
	}
	CLifeComponent* componentLife = NEW(CLifeComponent, bullet, weaponData.iBulletLife, weaponData.iBulletRange, 0);
	componentLife->init();
	return bullet;
}

Entity* CEntitiesFactory::createExplossion(vec2 _v2Pos, CWeaponComponent::EType _eWeaponType) {
	Entity* explossion = NEW(Entity, Entity::EWEAPON);

	vec2 v2Size = vmake(0.0f, 0.0f);
	vec2 v2SizeIncrement = vmake(0.0f, 0.0f);
	int iDuration = 0;
	int iColliderMask = 0;

	switch (_eWeaponType) {
		case CWeaponComponent::ENUCLEARBOMB:
			v2Size = vmake(20.0f, 20.0f);
			v2SizeIncrement = vmake(8.0f, 8.0f);
			iDuration = 100;
			iColliderMask = CColliderComponent::EPlayerWeaponCollider | CColliderComponent::EBoundariesCollider;
			break;
		default:
			v2Size = vmake(10.0f, 10.0f);
			v2SizeIncrement = vmake(2.0f, 2.0f);
			iDuration = 50;
			iColliderMask = CColliderComponent::EPlayerWeaponCollider | CColliderComponent::EEnemyWeaponCollider | CColliderComponent::EBoundariesCollider;
			break;
	}

	CRenderableComponent* renderable = NEW(CRenderableComponent, explossion, "data/explossion.png", 0.0f, 0.5f, 5);
	renderable->init();
	CTransformComponent* transform = NEW(CTransformComponent, explossion, _v2Pos, v2Size, v2SizeIncrement);
	transform->init();
	CColliderComponent* collider = NEW(CColliderComponent, explossion, CColliderComponent::ECircleCollider, -5, iColliderMask, CColliderComponent::ENoneCollider);
	collider->init();
	CLifeComponent* life = NEW(CLifeComponent, explossion, 1, iDuration, 0);
	life->init();
	
	g_pSoundEngine->playSound("data/explossion.wav");
	return explossion;
}

Entity* CEntitiesFactory::createEnemy(vec2 _v2Pos, Entity::EType _tEnemyType, Entity* _pPlayer) {
	Entity* enemy = NEW(Entity, _tEnemyType);
	TEnemyDef tEnemyDef = m_mEnemyDef[_tEnemyType];

	CTransformComponent* transform = NEW(CTransformComponent, enemy, _v2Pos, tEnemyDef.v2Size);
	transform->init();
	CRenderableComponent* renderable = NEW(CRenderableComponent, enemy, tEnemyDef.sImageFile.c_str(), 0.0f, 1.0f, 5, tEnemyDef.iInvencibleTime);
	renderable->init();

	// Melee and Big enemies follow player until contact
	if (tEnemyDef.eType == Entity::EENEMYMELEE || tEnemyDef.eType == Entity::EENEMYBIG) {
		CAIMeleeComponent* aiMelee = NEW(CAIMeleeComponent, enemy, tEnemyDef.fSpeed, 0);
		aiMelee->init();
	}

	// Range and Turrets have a fire weapon
	if (tEnemyDef.eWeapon != CWeaponComponent::EType::EInvalid) {
		CWeaponComponent* gun = NEW(CWeaponComponent, enemy, tEnemyDef.eWeapon);
		gun->init();

		// If a player is passed the enemy keep a distance between ComponentAIMelee and ComponentAIEvade distances and aim to it
		if (_pPlayer && tEnemyDef.eType != Entity::EENEMYBOSS) {
			CAIFireComponent* aiFire = NEW(CAIFireComponent, enemy);
			aiFire->init();
			CAIMeleeComponent* aiMelee = NEW(CAIMeleeComponent, enemy, tEnemyDef.fSpeed, 200);
			aiMelee->init();
			CAIFleeComponent* aiEvade = NEW(CAIFleeComponent, enemy, tEnemyDef.fSpeed, 150);
			aiEvade->init();
		}
	}

	// Boss AI
	if (tEnemyDef.eType == Entity::EENEMYBOSS) {
		BossIAComponent* bossAI = NEW(BossIAComponent, enemy, "data/bt/boss_bt.xml");
		bossAI->init();
		CExplossiveComponent* explossive = NEW(CExplossiveComponent, enemy);
		explossive->init();
	}

	CColliderComponent* collider = NEW(CColliderComponent, enemy, CColliderComponent::ERectCollider, tEnemyDef.iCollisionDamage, CColliderComponent::EEnemyCollider, CColliderComponent::EPlayerWeaponCollider);
	collider->init();
	CLifeComponent* life = NEW(CLifeComponent, enemy, tEnemyDef.iLife, 0, tEnemyDef.iInvencibleTime);
	life->init();
	CPointsComponent* points = NEW(CPointsComponent, enemy);
	points->init();
	return enemy;
}

Entity* CEntitiesFactory::createEnemy(vec2 _v2Pos, Entity::EType _tEnemyType, vec2 _v2MoveDir, std::vector<vec2> _vAimDirections, bool _bIshuffleAim) {
	Entity* enemy = createEnemy(_v2Pos, _tEnemyType, nullptr);
	TEnemyDef tEnemyDef = m_mEnemyDef[_tEnemyType];

	CMovementComponent* movement = NEW(CMovementComponent, enemy, _v2MoveDir, tEnemyDef.fSpeed, true, true);
	movement->init();

	// Used by the turrets to fire in the given directions and use a delay to not shoot all at the same time
	CAIFireComponent* aiFire = NEW(CAIFireComponent, enemy, _vAimDirections, _bIshuffleAim);
	aiFire->setActivationDelay(rand() % 100);
	aiFire->init();
	return enemy;
}

Entity* CEntitiesFactory::createWeaponPickup() {
	// Calculate a random weapon type
	CWeaponComponent::EType type = m_vWeaponPickups[rand() % m_vWeaponPickups.size()];
	// Calculate a random spawn position
	vec2 randomPos = vmake(CORE_FRand(0.0, WORLD_WIDTH), CORE_FRand(80, WORLD_HEIGHT - 80));

	Entity* weaponPickup = NEW(Entity, Entity::EPICKUP);
	CTransformComponent* transform = NEW(CTransformComponent, weaponPickup, randomPos, vmake(20, 20));
	transform->init();
	CRenderableComponent* renderable = NEW(CRenderableComponent, weaponPickup, "data/crate-1.png", 0.0f, 1.0f, 5);
	renderable->init();
	CColliderComponent* collider = NEW(CColliderComponent, weaponPickup, CColliderComponent::ERectCollider, 0, CColliderComponent::EPickupCollider, CColliderComponent::EPlayerCollider);
	collider->init();
	CWeaponPickupComponent* pickup = NEW(CWeaponPickupComponent, weaponPickup, type);
	pickup->init();
	CPointsComponent* points = NEW(CPointsComponent, weaponPickup);
	points->init();
	CLifeComponent* life = NEW(CLifeComponent, weaponPickup, 1, 0, 0);
	life->init();
	return weaponPickup;
}

//Entity* CEntitiesFactory::createHUDMessage(const std::string& _sMessage, int _iDisplayTime) {
//	Entity* hudMessage = NEW(Entity, Entity::EHUDMESSAGE);
//	CHUDMessageComponent* hudMessageComponent = NEW(CHUDMessageComponent, hudMessage, _sMessage);
//	hudMessageComponent->init();
//	CLifeComponent* life = NEW(CLifeComponent, hudMessage, 1, _iDisplayTime, 0);
//	life->init();
//	return hudMessage;
//}
