#include "common/stdafx.h"
#include "EntitiesFactory.h"

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
	// Image files
	const std::string s_sPlayerImage     = "data/images/player.png";
	const std::string s_sCrateImage      = "data/images/crate.png";
	const std::string s_sExplossionImage = "data/images/explossion.png";

	// Render priorities
	const int s_iExplossionRenderPriority = 5;
	const int s_iPlayerRenderPriority     = 6;
	const int s_iBulletsRenderPriority    = 7;
	const int s_iPickupsRenderPriority    = 8;
	const int s_iEnemiesRenderPriority    = 9;

	// 
	const float s_fPlayerInvencibleTime = 0.5f;
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
		weapon.eType = CWeaponComponent::getWeaponTypeByName(weapons[i]["name"].GetString());
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
		enemy.eType = CEntity::getEntityTypeByName(enemies[i]["type"].GetString());
		enemy.iLife = enemies[i]["life"].GetInt();
		enemy.fInvencibleTime = enemies[i]["invencibleTime"].GetFloat();
		enemy.fSpeed = enemies[i]["speed"].GetFloat();
		enemy.iCollisionDamage = enemies[i]["collisionDamage"].GetInt();
		enemy.eWeapon = CWeaponComponent::EType::EInvalid;
		if (enemies[i].HasMember("weapon")) {
			enemy.eWeapon = CWeaponComponent::getWeaponTypeByName(enemies[i]["weapon"].GetString());
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
		m_vWeaponPickups.push_back(CWeaponComponent::getWeaponTypeByName(pickups[i].GetString()));
	}

	fclose(file);

	return true;
}

CEntity* CEntitiesFactory::createPlayer(const vec2& _v2Pos) {
	CEntity* pPlayer = NEW(CEntity, CEntity::EPLAYER, _v2Pos, vmake(30.0f, 25.0f));

	CRenderableComponent* pRenderableComp = NEW(CRenderableComponent, pPlayer, s_sPlayerImage, 0.0f, 1.0f, s_iPlayerRenderPriority, false, true, s_fPlayerInvencibleTime);
	pRenderableComp->init();

	CPlayerControllerComponent* pPlayerControllerComp = NEW(CPlayerControllerComponent, pPlayer);
	pPlayerControllerComp->init();

	CMovementComponent* pMovementComp = NEW(CMovementComponent, pPlayer, vmake(0.0f, 0.0f), g_pWorld->getPlayerSpeed(), false);
	pMovementComp->init();

	CWeaponComponent* pWeaponComp = NEW(CWeaponComponent, pPlayer, CWeaponComponent::EREVOLVER);
	pWeaponComp->init();

	CColliderComponent* pColliderComp = NEW(CColliderComponent, pPlayer, CColliderComponent::ERectCollider, -1, CColliderComponent::EPlayerCollider, CColliderComponent::EEnemyCollider | CColliderComponent::EEnemyWeaponCollider);
	pColliderComp->init();

	CLifeComponent* pLifeComp = NEW(CLifeComponent, pPlayer, g_pWorld->getPlayerLife(), s_fPlayerInvencibleTime);
	pLifeComp->init();

	CHUDComponent* pHudComp = NEW(CHUDComponent, pPlayer);
	pHudComp->init();
	
	return pPlayer;
}

CEntity* CEntitiesFactory::createBullet(const vec2& _v2Pos, const CWeaponComponent::EType& _eWeaponType, const vec2& _v2Direction, const CEntity::EType& _eOwnerType) {
	if (!m_mWeaponDef.count(_eWeaponType)) {
		ASSERT(false, "Weapon definition not found");
		return nullptr;
	}
	TWeaponDef weaponData = m_mWeaponDef[_eWeaponType];
	
	CEntity* bullet = NEW(CEntity, CEntity::EWEAPON, _v2Pos, weaponData.v2BulletSize);

	CRenderableComponent* renderable = NEW(CRenderableComponent, bullet, weaponData.sImageFile, vangle(_v2Direction), 1.0f, s_iBulletsRenderPriority, true, false);
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

CEntity* CEntitiesFactory::createExplossion(const vec2& _v2Pos, const CWeaponComponent::EType& _eWeaponType) {
	// Defaults
	vec2 v2InitSize = vmake(10.0f, 10.0f);
	vec2 v2EndSize = vmake(110.0f, 110.0f);
	int iExplossionDamage = -5;
	float fDuration = 0.8f;
	int iColliderChannelMask = CColliderComponent::EPlayerWeaponCollider | CColliderComponent::EEnemyWeaponCollider | CColliderComponent::EBoundariesCollider;

	// Explossion specific
	switch (_eWeaponType) {
		case CWeaponComponent::ENUCLEARBOMB:
			v2InitSize = vmake(20.0f, 20.0f);
			v2EndSize = vmake(620.0f, 620.0f);
			fDuration = 1.6f;
			iColliderChannelMask = CColliderComponent::EPlayerWeaponCollider | CColliderComponent::EBoundariesCollider;
			break;
		default:
			break;
	}

	CEntity* pExplossion = NEW(CEntity, CEntity::EWEAPON, _v2Pos, v2InitSize);

	CRenderableComponent* pRenderableComp = NEW(CRenderableComponent, pExplossion, s_sExplossionImage, 0.0f, 0.5f, s_iExplossionRenderPriority);
	pRenderableComp->init();

	CColliderComponent* pColliderComp = NEW(CColliderComponent, pExplossion, CColliderComponent::ECircleCollider, iExplossionDamage, iColliderChannelMask, CColliderComponent::ENoneCollider);
	pColliderComp->init();

	CLifeComponent* pLifeComp = NEW(CLifeComponent, pExplossion, -1, 0.0f, fDuration);
	pLifeComp->init();

	CExplossionComponent* explossionComp = NEW(CExplossionComponent, pExplossion, v2InitSize, v2EndSize, fDuration);
	explossionComp->init();
	
	return pExplossion;
}

CEntity* CEntitiesFactory::createEnemy(const vec2& _v2Pos, const CEntity::EType& _eEnemyType, const std::string& _sBTFile, const vec2& _v2MoveDir, const vec2& _v2AimDir) {
	if (!m_mEnemyDef.count(_eEnemyType)) {
		ASSERT(false, "Enemy definition not found");
		return nullptr;
	}
	TEnemyDef tEnemyDef = m_mEnemyDef[_eEnemyType];
	
	// Defaults
	bool bAlignToMovement = false;
	bool bAlignToAim = false;
	bool bExplodeOnDeath = false;
	float fActivationDelay = 0.0f;
	std::string sBTFile = tEnemyDef.sBTFile;
	if (_sBTFile != "") {
		sBTFile = _sBTFile;
	}

	// Enemy specific
	switch (tEnemyDef.eType) {
		case CEntity::EENEMYRANGE:
			bAlignToAim = true;
			bExplodeOnDeath = true;
			break;
		case CEntity::EENEMYTURRET:
			bAlignToAim = true;
			bExplodeOnDeath = true;
			break;
		case CEntity::EENEMYBOSS:
			bExplodeOnDeath = true;
			break;
		default:
			break;
	}

	CEntity* pEnemy = NEW(CEntity, _eEnemyType, _v2Pos, tEnemyDef.v2Size);

	CMovementComponent * pMovementComp = NEW(CMovementComponent, pEnemy, _v2MoveDir, tEnemyDef.fSpeed, true);
	pMovementComp->init();

	CRenderableComponent* pRenderableComp = NEW(CRenderableComponent, pEnemy, tEnemyDef.sImageFile.c_str(), 0.0f, 1.0f, s_iEnemiesRenderPriority, bAlignToMovement, bAlignToAim, 0.5f);
	pRenderableComp->init();

	CColliderComponent* pColliderComp = NEW(CColliderComponent, pEnemy, CColliderComponent::ERectCollider, tEnemyDef.iCollisionDamage, CColliderComponent::EEnemyCollider, CColliderComponent::EPlayerWeaponCollider);
	pColliderComp->init();

	CLifeComponent* pLifeComp = NEW(CLifeComponent, pEnemy, tEnemyDef.iLife, tEnemyDef.fInvencibleTime);
	pLifeComp->init();

	CPointsComponent* pPointsComp = NEW(CPointsComponent, pEnemy);
	pPointsComp->init();
	
	CBehaviorTreeComponent * pBTComp = NEW(CBehaviorTreeComponent, pEnemy, sBTFile.c_str());
	pBTComp->setActivationDelay(fActivationDelay);
	pBTComp->init();

	if (tEnemyDef.eWeapon != CWeaponComponent::EType::EInvalid) {
		CWeaponComponent* pWeaponComp = NEW(CWeaponComponent, pEnemy, tEnemyDef.eWeapon, _v2AimDir);
		pWeaponComp->init();
	}

	if (bExplodeOnDeath) {
		CExplossiveComponent* pExplossiveComp = NEW(CExplossiveComponent, pEnemy);
		pExplossiveComp->init();
	}

	return pEnemy;
}

CEntity* CEntitiesFactory::createWeaponPickup() {
	// Choose a random weapon type
	CWeaponComponent::EType eWeaponType = m_vWeaponPickups[rand() % m_vWeaponPickups.size()];

	// Choose a random spawn position
	vec2 v2RandomPos = vmake(CORE_FRand(0.0f, WORLD_WIDTH), CORE_FRand(80.0f, WORLD_HEIGHT - 80.0f));

	CEntity* pWeaponPickup = NEW(CEntity, CEntity::EPICKUP, v2RandomPos, vmake(20.0f, 20.0f));

	CRenderableComponent* pRenderableComp = NEW(CRenderableComponent, pWeaponPickup, s_sCrateImage, 0.0f, 1.0f, s_iPickupsRenderPriority);
	pRenderableComp->init();

	CColliderComponent* pColliderComp = NEW(CColliderComponent, pWeaponPickup, CColliderComponent::ERectCollider, 0, CColliderComponent::EPickupCollider, CColliderComponent::EPlayerCollider);
	pColliderComp->init();

	CWeaponPickupComponent* pPickupComp = NEW(CWeaponPickupComponent, pWeaponPickup, eWeaponType);
	pPickupComp->init();

	CLifeComponent* pLifeComp = NEW(CLifeComponent, pWeaponPickup, 1);
	pLifeComp->init();

	CPointsComponent* pPointsComp = NEW(CPointsComponent, pWeaponPickup);
	pPointsComp->init();

	return pWeaponPickup;
}
