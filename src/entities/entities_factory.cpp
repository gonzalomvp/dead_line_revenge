#include "common/stdafx.h"
#include "entities_factory.h"

#include "engine/sound_engine.h"

#include "components/LifeComponent.h"
#include "components/MovementComponent.h"
#include "components/TransformComponent.h"
#include "components/bossIAComponent.h"

#include "scenes/world.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

CEntitiesFactory::TEntityInfo CEntitiesFactory::s_aEntityInfo[] = {
#define REG_ENTITY(val, name) \
	{Entity::E##val, name},
#include "REG_ENTITIES.h"
#undef REG_ENTITY
};

Entity::EType CEntitiesFactory::getEntityTypeByName(const std::string& name) {
	Entity::EType etype = Entity::EInvalid;
	int i = 0;
	while ((etype == Entity::EInvalid) && (i < Entity::NUM_ENTITIES))
	{
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
		ComponentWeapon::TWeaponData weapon;
		weapon.type = ComponentWeapon::getWeaponTypeByName(weapons[i]["name"].GetString());
		weapon.fireRate = weapons[i]["fireRate"].GetInt();
		weapon.reloadTime = weapons[i]["reloadTime"].GetInt();
		weapon.capacity = weapons[i]["capacity"].GetInt();
		weapon.bulletSpeed = weapons[i]["bulletSpeed"].GetFloat();
		weapon.bulletDamage = weapons[i]["bulletDamage"].GetInt();
		weapon.bulletLife = weapons[i]["bulletLife"].GetInt();
		weapon.bulletRange = weapons[i]["bulletRange"].GetInt();
		weapon.numBullets = 1;
		if (weapons[i].HasMember("numBullets")) {
			weapon.numBullets = weapons[i]["numBullets"].GetUint();
		}
		weapon.isAutomatic = weapons[i]["isAutomatic"].GetBool();
		weapon.isExplossive = weapons[i]["isExplossive"].GetBool();
		weapon.isBouncy = weapons[i]["isBouncy"].GetBool();
		weapon.soundFile = weapons[i]["soundFile"].GetString();
		m_mWeaponDef[weapon.type] = weapon;
	}

	// Load enemy definitions
	const Value& enemies = doc["enemies"];
	for (SizeType i = 0; i < enemies.Size(); i++) {
		TEnemyDef enemy;
		enemy.eType = getEntityTypeByName(enemies[i]["type"].GetString());
		enemy.iLife = enemies[i]["life"].GetInt();
		enemy.fSpeed = enemies[i]["speed"].GetFloat();
		enemy.iCollisionDamage = enemies[i]["collisionDamage"].GetInt();
		enemy.eWeapon = ComponentWeapon::EType::EInvalid;
		if (enemies[i].HasMember("weapon")) {
			enemy.eWeapon = ComponentWeapon::getWeaponTypeByName(enemies[i]["weapon"].GetString());
		}
		enemy.v2Size = vmake(enemies[i]["size"][0].GetFloat(), enemies[i]["size"][1].GetFloat());
		enemy.sImageFile = enemies[i]["imageFile"].GetString();
		m_mEnemyDef[enemy.eType] = enemy;
	}

	// Load weapon pickups
	const Value& pickups = doc["pickups"];
	for (SizeType i = 0; i < pickups.Size(); i++) {
		m_vWeaponPickups.push_back(ComponentWeapon::getWeaponTypeByName(pickups[i].GetString()));
	}

	fclose(file);

	return true;
}

Entity* CEntitiesFactory::createPlayer(vec2 _v2Pos) {
	Entity* player = NEW(Entity, Entity::EPLAYER);
	CTransformComponent* transform = NEW(CTransformComponent, player, _v2Pos, vmake(30, 25));
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, player, "data/player.png", 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentPlayerController* playerControl = NEW(ComponentPlayerController, player);
	playerControl->init();
	CMovementComponent* movement = NEW(CMovementComponent, player, vmake(0.0f, 0.0f), g_pWorld->getPlayerSpeed(), false, false);
	movement->init();
	ComponentWeapon* weapon = NEW(ComponentWeapon, player, m_mWeaponDef[ComponentWeapon::EREVOLVER]);
	weapon->init();
	ComponentCollider* collider = NEW(ComponentCollider, player, ComponentCollider::ERectCollider, -1, ComponentCollider::EPlayerCollider, ComponentCollider::EEnemyCollider | ComponentCollider::EEnemyWeaponCollider);
	collider->init();
	CLifeComponent* life = NEW(CLifeComponent, player, g_pWorld->getPlayerLife(), 0, 20);
	life->init();
	ComponentHUD* hudComponent = NEW(ComponentHUD, player);
	hudComponent->init();
	return player;
}

Entity* CEntitiesFactory::createBullet(vec2 _v2Pos, vec2 _v2Size, vec2 _v2Direction, float _fSpeed, int _iDamage, int _iLife, int _iRange, bool _bIsExplossive, bool _bIsBouncy, Entity::EType _eEntityType, const char* _psTexture) {
	Entity* bullet = NEW(Entity, Entity::EWEAPON);
	CTransformComponent* transform = NEW(CTransformComponent, bullet, _v2Pos, _v2Size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, bullet, _psTexture, vangle(_v2Direction), 1.0f, 5);
	renderable->init();
	CMovementComponent* movement = NEW(CMovementComponent, bullet, _v2Direction, _fSpeed, true, _bIsBouncy);
	movement->init();

	// Depending on the type of bullet it has different collider setup
	switch (_eEntityType) {
	case Entity::EPLAYER: {
		ComponentCollider* collider = NEW(ComponentCollider, bullet, ComponentCollider::ECircleCollider, _iDamage, ComponentCollider::EPlayerWeaponCollider, ComponentCollider::EEnemyCollider | ComponentCollider::EBoundariesCollider);
		collider->init();
		break;
	}
	case Entity::EMINE: {
		ComponentCollider* collider = NEW(ComponentCollider, bullet, ComponentCollider::ECircleCollider, 0, ComponentCollider::ENoneCollider, ComponentCollider::EPlayerCollider | ComponentCollider::EEnemyCollider | ComponentCollider::EPlayerWeaponCollider | ComponentCollider::EEnemyWeaponCollider);
		collider->setActivationDelay(20);
		collider->init();
		break;
	}
	default: {
		ComponentCollider* collider = NEW(ComponentCollider, bullet, ComponentCollider::ECircleCollider, _iDamage, ComponentCollider::EEnemyWeaponCollider, ComponentCollider::EPlayerCollider | ComponentCollider::EBoundariesCollider);
		collider->init();
		break;
	}
	}
	if (_bIsExplossive) {
		ComponentExplossive* explossive = NEW(ComponentExplossive, bullet);
		explossive->init();
	}
	CLifeComponent* componentLife = NEW(CLifeComponent, bullet, _iLife, _iRange, 0);
	componentLife->init();
	return bullet;
}

Entity* CEntitiesFactory::createExplossion(vec2 _v2Pos, vec2 _v2Size, vec2 _v2SizeIncrement, int _iDuration, Entity::EType _eEntityType) {
	Entity* explossion = NEW(Entity, _eEntityType);
	CTransformComponent* transform = NEW(CTransformComponent, explossion, _v2Pos, _v2Size, _v2SizeIncrement);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, explossion, "data/explossion.png", 0.0f, 0.5f, 5);
	renderable->init();

	// Nuclear explossion has different collider than standard explosssion
	switch (_eEntityType) {
	case Entity::ENUCLEAREXPLOSSION: {
		ComponentCollider* collider = NEW(ComponentCollider, explossion, ComponentCollider::ECircleCollider, -50, ComponentCollider::EPlayerWeaponCollider | ComponentCollider::EBoundariesCollider, ComponentCollider::ENoneCollider);
		collider->init();
		break;
	}
	default: {
		ComponentCollider* collider = NEW(ComponentCollider, explossion, ComponentCollider::ECircleCollider, -1, ComponentCollider::EPlayerWeaponCollider | ComponentCollider::EEnemyWeaponCollider | ComponentCollider::EBoundariesCollider, ComponentCollider::ENoneCollider);
		collider->init();
		break;
	}
	}
	CLifeComponent* life = NEW(CLifeComponent, explossion, 1, _iDuration, 0);
	life->init();
	g_pSoundEngine->playSound("data/explossion.wav");
	return explossion;
}

Entity* CEntitiesFactory::createEnemy(vec2 _v2Pos, Entity::EType _tEnemyType, Entity* _pPlayer) {
	Entity* enemy = NEW(Entity, _tEnemyType);
	TEnemyDef tEnemyDef = m_mEnemyDef[_tEnemyType];

	CTransformComponent* transform = NEW(CTransformComponent, enemy, _v2Pos, tEnemyDef.v2Size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, enemy, tEnemyDef.sImageFile.c_str(), 0.0f, 1.0f, 5, 10);
	renderable->init();

	// Melee and Big enemies follow player until contact
	if (tEnemyDef.eType == Entity::EENEMYMELEE || tEnemyDef.eType == Entity::EENEMYBIG) {
		ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, _pPlayer, tEnemyDef.fSpeed, 0);
		aiMelee->init();
	}

	// Range and Turrets have a fire weapon
	if (tEnemyDef.eWeapon != ComponentWeapon::EType::EInvalid) {
		ComponentWeapon* gun = NEW(ComponentWeapon, enemy, m_mWeaponDef[tEnemyDef.eWeapon]);
		gun->init();

		// If a player is passed the enemy keep a distance between ComponentAIMelee and ComponentAIEvade distances and aim to it
		if (_pPlayer && tEnemyDef.eType != Entity::EENEMYBOSS) {
			ComponentAIFire* aiFire = NEW(ComponentAIFire, enemy, _pPlayer);
			aiFire->init();
			ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, _pPlayer, tEnemyDef.fSpeed, 200);
			aiMelee->init();
			ComponentAIEvade* aiEvade = NEW(ComponentAIEvade, enemy, _pPlayer, tEnemyDef.fSpeed, 150);
			aiEvade->init();
		}
	}

	// Boss AI
	if (tEnemyDef.eType == Entity::EENEMYBOSS) {
		BossIAComponent* bossAI = NEW(BossIAComponent, enemy, "data/bt/boss_bt.xml");
		bossAI->init();
		ComponentExplossive* explossive = NEW(ComponentExplossive, enemy);
		explossive->init();
	}

	ComponentCollider* collider = NEW(ComponentCollider, enemy, ComponentCollider::ERectCollider, tEnemyDef.iCollisionDamage, ComponentCollider::EEnemyCollider, ComponentCollider::EPlayerWeaponCollider);
	collider->init();
	CLifeComponent* life = NEW(CLifeComponent, enemy, tEnemyDef.iLife, 0, 0);
	life->init();
	ComponentPoints* points = NEW(ComponentPoints, enemy);
	points->init();
	return enemy;
}

Entity* CEntitiesFactory::createEnemy(vec2 _v2Pos, Entity::EType _tEnemyType, vec2 _v2MoveDir, std::vector<vec2> _vAimDirections, bool _bIshuffleAim) {
	Entity* enemy = createEnemy(_v2Pos, _tEnemyType, nullptr);
	TEnemyDef tEnemyDef = m_mEnemyDef[_tEnemyType];

	CMovementComponent* movement = NEW(CMovementComponent, enemy, _v2MoveDir, tEnemyDef.fSpeed, true, true);
	movement->init();

	// Used by the turrets to fire in the given directions and use a delay to not shoot all at the same time
	ComponentAIFire* aiFire = NEW(ComponentAIFire, enemy, _vAimDirections, _bIshuffleAim);
	aiFire->setActivationDelay(rand() % 100);
	aiFire->init();
	return enemy;
}

Entity* CEntitiesFactory::createWeaponPickup() {
	// Calculate a random weapon type
	ComponentWeapon::EType type = m_vWeaponPickups[rand() % m_vWeaponPickups.size()];
	// Calculate a random spawn position
	vec2 randomPos = vmake(CORE_FRand(0.0, WORLD_WIDTH), CORE_FRand(80, WORLD_HEIGHT - 80));

	Entity* weaponPickup = NEW(Entity, Entity::EPICKUP);
	CTransformComponent* transform = NEW(CTransformComponent, weaponPickup, randomPos, vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, weaponPickup, "data/crate-1.png", 0.0f, 1.0f, 5);
	renderable->init();
	ComponentCollider* collider = NEW(ComponentCollider, weaponPickup, ComponentCollider::ERectCollider, 0, ComponentCollider::EPickupCollider, ComponentCollider::EPlayerCollider);
	collider->init();
	ComponentWeaponPickup* pickup = NEW(ComponentWeaponPickup, weaponPickup, m_mWeaponDef[type]);
	pickup->init();
	ComponentPoints* points = NEW(ComponentPoints, weaponPickup);
	points->init();
	CLifeComponent* life = NEW(CLifeComponent, weaponPickup, 1, 0, 0);
	life->init();
	return weaponPickup;
}

Entity* CEntitiesFactory::createHUDMessage(const std::string& _sMessage, vec2 _v2Pos, int _iDisplayTime) {
	Entity* hudMessage = NEW(Entity, Entity::EHUDMESSAGE);
	ComponentHUDMessage* hudMessageComponent = NEW(ComponentHUDMessage, hudMessage, _v2Pos, _sMessage);
	hudMessageComponent->init();
	CLifeComponent* life = NEW(CLifeComponent, hudMessage, 1, _iDisplayTime, 0);
	life->init();
	return hudMessage;
}
