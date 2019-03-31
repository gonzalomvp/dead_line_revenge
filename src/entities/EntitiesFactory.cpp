#include "common/stdafx.h"
#include "EntitiesFactory.h"

#include "engine/SoundEngine.h"

#include "components/BehaviorTreeComponent.h"
#include "components/ColliderComponent.h"
#include "components/ExplossionComponent.h"
#include "components/ExplossiveComponent.h"
#include "components/HUDComponent.h"
#include "components/LifeComponent.h"
#include "components/MovementComponent.h"
#include "components/PlayerControllerComponent.h"
#include "components/PointsComponent.h"
#include "components/RenderableComponent.h"
#include "components/WeaponComponent.h"
#include "components/WeaponPickupComponent.h"

#include "scenes/World.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

namespace {
	const std::string s_sPlayerImage     = "data/images/player.png";
	const std::string s_sCrateImage      = "data/images/crate.png";
	const std::string s_sExplossionImage = "data/images/explossion.png";
}

CEntitiesFactory::TWeaponInfo CEntitiesFactory::s_aWeaponInfo[] = {
#define REG_WEAPON(val, name) \
	{CWeaponComponent::E##val, name},
#include "REG_WEAPONS.h"
#undef REG_WEAPON
};

CEntitiesFactory::TEntityInfo CEntitiesFactory::s_aEntityInfo[] = {
#define REG_ENTITY(val, name) \
	{CEntity::E##val, name},
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


CEntity::EType CEntitiesFactory::getEntityTypeByName(const std::string& name) {
	CEntity::EType etype = CEntity::EInvalid;
	int i = 0;
	while ((etype == CEntity::EInvalid) && (i < CEntity::NUM_ENTITIES)) {
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
		weapon.fFireRate = weapons[i]["fireRate"].GetFloat();
		weapon.fReloadTime = weapons[i]["reloadTime"].GetFloat();
		weapon.iMaxBullets = weapons[i]["maxBullets"].GetInt();
		weapon.fBulletSpeed = weapons[i]["bulletSpeed"].GetFloat();
		weapon.iBulletDamage = weapons[i]["bulletDamage"].GetInt();
		weapon.iBulletLife = weapons[i]["bulletLife"].GetInt();
		weapon.fBulletRange = weapons[i]["bulletRange"].GetFloat();

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
		enemy.fInvencibleTime = enemies[i]["invencibleTime"].GetFloat();
		enemy.fSpeed = enemies[i]["speed"].GetFloat();
		enemy.iCollisionDamage = enemies[i]["collisionDamage"].GetInt();
		enemy.eWeapon = CWeaponComponent::EType::EInvalid;
		if (enemies[i].HasMember("weapon")) {
			enemy.eWeapon = getWeaponTypeByName(enemies[i]["weapon"].GetString());
		}
		enemy.v2Size = vmake(enemies[i]["size"][0].GetFloat(), enemies[i]["size"][1].GetFloat());
		enemy.sImageFile = enemies[i]["imageFile"].GetString();
		enemy.sBTFile == "";
		if (enemies[i].HasMember("behaviorTree")) {
			enemy.sBTFile = enemies[i]["behaviorTree"].GetString();
		}
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

CEntity* CEntitiesFactory::createPlayer(vec2 _v2Pos) {
	CEntity* player = NEW(CEntity, CEntity::EPLAYER, _v2Pos, vmake(30.0f, 25.0f));
	//CTransformComponent* transform = NEW(CTransformComponent, player, _v2Pos, vmake(30, 25));
	//transform->init();
	CRenderableComponent* renderable = NEW(CRenderableComponent, player, s_sPlayerImage, 0.0f, 1.0f, 5, false, true, 0.5f);
	renderable->init();
	CPlayerControllerComponent* playerControl = NEW(CPlayerControllerComponent, player);
	playerControl->init();
	CMovementComponent* movement = NEW(CMovementComponent, player, vmake(0.0f, 0.0f), g_pWorld->getPlayerSpeed(), false);
	movement->init();
	CWeaponComponent* weapon = NEW(CWeaponComponent, player, CWeaponComponent::EREVOLVER);
	weapon->init();
	CColliderComponent* collider = NEW(CColliderComponent, player, CColliderComponent::ERectCollider, -1, CColliderComponent::EPlayerCollider, CColliderComponent::EEnemyCollider | CColliderComponent::EEnemyWeaponCollider);
	collider->init();
	CLifeComponent* life = NEW(CLifeComponent, player, g_pWorld->getPlayerLife(), 0.5f);
	life->init();
	CHUDComponent* hudComponent = NEW(CHUDComponent, player);
	hudComponent->init();
	return player;
}

CEntity* CEntitiesFactory::createBullet(CWeaponComponent::EType _eWeaponType, vec2 _v2Pos, vec2 _v2Direction, CEntity::EType _eOwnerType) {
	if (!m_mWeaponDef.count(_eWeaponType)) {
		ASSERT(false, "Weapon definition not found");
		return nullptr;
	}
	TWeaponDef weaponData = m_mWeaponDef[_eWeaponType];
	
	CEntity* bullet = NEW(CEntity, CEntity::EWEAPON, _v2Pos, weaponData.v2BulletSize);

	CRenderableComponent* renderable = NEW(CRenderableComponent, bullet, weaponData.sImageFile, vangle(_v2Direction), 1.0f, 6, true, false);
	renderable->init();
	CMovementComponent* movement = NEW(CMovementComponent, bullet, _v2Direction, weaponData.fBulletSpeed, weaponData.bIsBouncy);
	movement->init();

	// Depending on the type of bullet it has different collider setup
	int iColliderChannelMask = CColliderComponent::ENoneCollider;
	int iColliderChannelMaskResponse = CColliderComponent::ENoneCollider;
	float fActivationDelay = 0.0f;

	switch (_eWeaponType) {
		case CWeaponComponent::EMINES:
			iColliderChannelMask = CColliderComponent::ENoneCollider;
			iColliderChannelMaskResponse = CColliderComponent::EPlayerCollider | CColliderComponent::EEnemyCollider | CColliderComponent::EPlayerWeaponCollider | CColliderComponent::EEnemyWeaponCollider;
			fActivationDelay = 0.4f;
			break;
		case CWeaponComponent::EC4:
			iColliderChannelMask = CColliderComponent::ENoneCollider;
			iColliderChannelMaskResponse = CColliderComponent::ENoneCollider;
			break;
		default:
			switch (_eOwnerType) {
				case CEntity::EPLAYER:
					iColliderChannelMask = CColliderComponent::EPlayerWeaponCollider;
					iColliderChannelMaskResponse = CColliderComponent::EEnemyCollider | CColliderComponent::EBoundariesCollider;
					break;
				default:
					iColliderChannelMask = CColliderComponent::EEnemyWeaponCollider;
					iColliderChannelMaskResponse = CColliderComponent::EPlayerCollider | CColliderComponent::EBoundariesCollider;
					break;
			}
			break;
	}
	CColliderComponent* collider = NEW(CColliderComponent, bullet, CColliderComponent::ECircleCollider, weaponData.iBulletDamage, iColliderChannelMask, iColliderChannelMaskResponse);
	collider->setActivationDelay(fActivationDelay);
	collider->init();

	if (weaponData.bIsExplossive) {
		CExplossiveComponent* explossive = NEW(CExplossiveComponent, bullet);
		explossive->init();
	}
	CLifeComponent* componentLife = NEW(CLifeComponent, bullet, weaponData.iBulletLife, 0.0f, weaponData.fBulletRange);
	componentLife->init();
	return bullet;
}

CEntity* CEntitiesFactory::createExplossion(vec2 _v2Pos, CWeaponComponent::EType _eWeaponType) {
	vec2 v2InitSize = vmake(0.0f, 0.0f);
	vec2 v2EndSize = vmake(0.0f, 0.0f);
	float fDuration = 0.0f;
	int iColliderChannelMask = CColliderComponent::ENoneCollider;

	switch (_eWeaponType) {
		case CWeaponComponent::ENUCLEARBOMB:
			v2InitSize = vmake(20.0f, 20.0f);
			v2EndSize = vmake(620.0f, 620.0f);
			fDuration = 1.6f;
			iColliderChannelMask = CColliderComponent::EPlayerWeaponCollider | CColliderComponent::EBoundariesCollider;
			break;
		default:
			v2InitSize = vmake(10.0f, 10.0f);
			v2EndSize = vmake(110.0f, 110.0f);
			fDuration = 0.8f;
			iColliderChannelMask = CColliderComponent::EPlayerWeaponCollider | CColliderComponent::EEnemyWeaponCollider | CColliderComponent::EBoundariesCollider;
			break;
	}

	CEntity* explossion = NEW(CEntity, CEntity::EWEAPON, _v2Pos, v2InitSize);

	CRenderableComponent* renderable = NEW(CRenderableComponent, explossion, s_sExplossionImage, 0.0f, 0.5f, 5);
	renderable->init();
	CColliderComponent* collider = NEW(CColliderComponent, explossion, CColliderComponent::ECircleCollider, -5, iColliderChannelMask, CColliderComponent::ENoneCollider);
	collider->init();
	CLifeComponent* life = NEW(CLifeComponent, explossion, 1, 0.0f, fDuration);
	life->init();
	CExplossionComponent* explossionComp = NEW(CExplossionComponent, explossion, v2InitSize, v2EndSize, fDuration);
	explossionComp->init();
	
	return explossion;
}

CEntity* CEntitiesFactory::createEnemy(vec2 _v2Pos, CEntity::EType _tEnemyType, const std::string& _sBTFile, vec2 _v2MoveDir, vec2 _vAimDir) {
	TEnemyDef tEnemyDef = m_mEnemyDef[_tEnemyType];

	CEntity* enemy = NEW(CEntity, _tEnemyType, _v2Pos, tEnemyDef.v2Size);
	
	bool bAlignToMovement = false;
	bool bAlignToAim = false;
	std::string sBTFile = tEnemyDef.sBTFile;
	if (_sBTFile != "") {
		sBTFile = _sBTFile;
	}

	CMovementComponent * movement = NEW(CMovementComponent, enemy, _v2MoveDir, tEnemyDef.fSpeed, true);
	movement->init();
	CColliderComponent* collider = NEW(CColliderComponent, enemy, CColliderComponent::ERectCollider, tEnemyDef.iCollisionDamage, CColliderComponent::EEnemyCollider, CColliderComponent::EPlayerWeaponCollider);
	collider->init();
	CLifeComponent* life = NEW(CLifeComponent, enemy, tEnemyDef.iLife, tEnemyDef.fInvencibleTime);
	life->init();
	CPointsComponent* points = NEW(CPointsComponent, enemy);
	points->init();

	if (tEnemyDef.eWeapon != CWeaponComponent::EType::EInvalid) {
		CWeaponComponent* gun = NEW(CWeaponComponent, enemy, tEnemyDef.eWeapon, _vAimDir);
		gun->init();
	}

	switch (tEnemyDef.eType)
	{
	case CEntity::EENEMYMELEE: {
		CBehaviorTreeComponent * pBTComponent = NEW(CBehaviorTreeComponent, enemy, sBTFile.c_str());
		pBTComponent->init();
		break;
	}
	case CEntity::EENEMYBIG: {
		CBehaviorTreeComponent * pBTComponent = NEW(CBehaviorTreeComponent, enemy, sBTFile.c_str());
		pBTComponent->init();
		break;
	}	
	case CEntity::EENEMYRANGE: {
		bAlignToAim = true;
		CBehaviorTreeComponent * pBTComponent = NEW(CBehaviorTreeComponent, enemy, sBTFile.c_str());
		pBTComponent->init();
		CExplossiveComponent* explossive = NEW(CExplossiveComponent, enemy);
		explossive->init();
		break;
	}
	case CEntity::EENEMYTURRET: {
		bAlignToAim = true;
		CBehaviorTreeComponent * pBTComponent = NEW(CBehaviorTreeComponent, enemy, sBTFile.c_str());
		pBTComponent->setActivationDelay(CORE_FRand(1.0f, 2.0f));
		pBTComponent->init();
		CExplossiveComponent* explossive = NEW(CExplossiveComponent, enemy);
		explossive->init();
		break;
	}
	case CEntity::EENEMYBOSS: {
		CBehaviorTreeComponent * pBTComponent = NEW(CBehaviorTreeComponent, enemy, sBTFile.c_str());
		pBTComponent->init();
		CExplossiveComponent* explossive = NEW(CExplossiveComponent, enemy);
		explossive->init();
		break;
	}
	default:
		break;
	}

	CRenderableComponent* renderable = NEW(CRenderableComponent, enemy, tEnemyDef.sImageFile.c_str(), 0.0f, 1.0f, 8, bAlignToMovement, bAlignToAim, 0.5f);
	renderable->init();

	return enemy;
}

CEntity* CEntitiesFactory::createWeaponPickup() {
	// Calculate a random weapon type
	CWeaponComponent::EType type = m_vWeaponPickups[rand() % m_vWeaponPickups.size()];
	// Calculate a random spawn position
	vec2 randomPos = vmake(CORE_FRand(0.0, WORLD_WIDTH), CORE_FRand(80, WORLD_HEIGHT - 80));

	CEntity* weaponPickup = NEW(CEntity, CEntity::EPICKUP, randomPos, vmake(20, 20));
	CRenderableComponent* renderable = NEW(CRenderableComponent, weaponPickup, s_sCrateImage, 0.0f, 1.0f, 7);
	renderable->init();
	CColliderComponent* collider = NEW(CColliderComponent, weaponPickup, CColliderComponent::ERectCollider, 0, CColliderComponent::EPickupCollider, CColliderComponent::EPlayerCollider);
	collider->init();
	CWeaponPickupComponent* pickup = NEW(CWeaponPickupComponent, weaponPickup, type);
	pickup->init();
	CPointsComponent* points = NEW(CPointsComponent, weaponPickup);
	points->init();
	CLifeComponent* life = NEW(CLifeComponent, weaponPickup, 1);
	life->init();
	return weaponPickup;
}
