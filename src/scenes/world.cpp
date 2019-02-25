#include "common/stdafx.h"
#include "scenes/world.h"

#include "engine/sound_engine.h"
#include "entities/components/component.h"
#include "entities/components/bossIAComponent.h"
#include "entities/message.h"
#include "gui/string_manager.h"
#include "gui/menu.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

CWorld::~CWorld() {
	unloadLevel();
	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::TEventType::EPause);
	}
}

void CWorld::init() {
	ASSERT(g_pInputManager);
	g_pInputManager->registerEvent(this, IInputManager::TEventType::EPause);
	loadConfig();
}

bool CWorld::loadLevel() {
	m_pPlayer = nullptr;
	m_pHudMessage = nullptr;
	m_bIsGameOver = false;
	m_bIsPaused = false;
	m_uScore = 0;
	m_iCurrentEnemies = 0;
	m_iPickupSpawnTimer = 0;
	m_iEnemySpawnTimer = 0;

	// Parse level file
	char* fileName = "";
	switch (m_uLevel) {
		case 1:
			fileName = "data/level1.json";
			break;
		case 2:
			fileName = "data/level2.json";
			break;
		case 3:
			fileName = "data/level3.json";
			break;
	}

	// Force test level
	fileName = "data/levelTest.json";

	FILE* file = fopen(fileName, "r");
	if (!file) return false;

	fseek(file, 0, SEEK_END);
	std::vector<char> fileData(ftell(file));
	fseek(file, 0, SEEK_SET);
	FileReadStream is(file, fileData.data(), fileData.size());
	Document doc;
	doc.ParseStream(is);

	// Load general level rules
	m_iPlayerLife       = doc["playerLife"].GetInt();
	m_fPlayerSpeed      = doc["playerSpeed"].GetFloat();
	m_iPickupPoints     = doc["pickupPoints"].GetInt();
	m_iPickupSpawnWait  = doc["pickupSpawnWait"].GetInt();
	m_iEnemySpawnWait   = doc["enemySpawnWait"].GetInt();
	m_iMaxEnemies       = doc["maxEnemies"].GetInt();
	m_iPickupSpawnTimer = m_iPickupSpawnWait;

	// Load enemy level configuration
	float totalProbability = 0.0f;
	const Value& enemies   = doc["enemies"];
	for (SizeType i = 0; i < enemies.Size(); i++) {
		TEnemyData enemy          = m_mEnemyData[static_cast<Entity::TType>(enemies[i]["id"].GetInt())];
		totalProbability          += enemies[i]["spawnProbability"].GetFloat();
		enemy.fSpawnProbability   = totalProbability;
		enemy.iPoints             = enemies[i]["points"].GetInt();
		m_mEnemyData[enemy.eType] = enemy;
	}

	// Load turret level configuration
	const Value& turrets = doc["turrets"];
	for (SizeType i = 0; i < turrets.Size(); i++) {
		std::vector<vec2> aimDirections;
		vec2 pos                = vmake(turrets[i]["position"][0].GetFloat(), turrets[i]["position"][1].GetFloat());
		vec2 moveDirection      = vmake(turrets[i]["moveDirection"][0].GetFloat(), turrets[i]["moveDirection"][1].GetFloat());
		bool shuffleAim         = turrets[i]["shuffleAim"].GetBool();
		const Value& directions = turrets[i]["aimDirections"];
		for (SizeType j = 0; j < directions.Size(); j++) {
			aimDirections.push_back(vmake(directions[j][0].GetFloat(), directions[j][1].GetFloat()));
		}
		
		// Create turrets
		createEnemy(pos, m_mEnemyData[Entity::ETurret], moveDirection, aimDirections, shuffleAim);
		++m_iCurrentEnemies;
	}
	fclose(file);

	// Create player and first pickup
	createPlayer(vmake(WORLD_WIDTH * 0.5f, WORLD_HEIGHT * 0.5f));
	createWeaponPickup();

	return true;
}

void CWorld::unloadLevel() {
	for (size_t i = 0; i < m_vEntities.size(); ++i) {
		DELETE(m_vEntities[i]);
	}
	m_vEntities.clear();

	for (size_t i = 0; i < m_vEntitiesToAdd.size(); ++i) {
		DELETE(m_vEntitiesToAdd[i]);
	}
	m_vEntitiesToAdd.clear();

	m_vEntitiesToRemove.clear();
}

void CWorld::run(float _fDeltaTime) {
	if (!m_bIsPaused && !m_bIsGameOver) {

		for (size_t i = 0; i < m_vEntities.size(); ++i) {
			m_vEntities[i]->run(_fDeltaTime);
		}
		checkCollisions();
		removePendingEntities();
		spawnNewEntities();
		addPendingEntities();
	}
}

void CWorld::addEntity(Entity* _pEntity) {
	m_vEntitiesToAdd.push_back(_pEntity);
}

void CWorld::removeEntity(Entity* _pEntity) {
	m_vEntitiesToRemove.push_back(_pEntity);
}

bool CWorld::onEvent(const IInputManager::Event& _event) {
	if (m_bIsGameOver)
		return true;

	IInputManager::TEventType eventType = _event.getType();
	if (eventType == IInputManager::TEventType::EPause) {
		m_bIsPaused = !m_bIsPaused;
		if (m_bIsPaused) {
			m_pPlayer->deactivate();
			g_pMenuManager->activateMenu(MenuManager::EPauseMenu);
		}
		else {
			m_pPlayer->activate();
			g_pMenuManager->deactivateMenu();
		}
	}
	return true;
}

//=============================================================================
// Entity creation methods
//=============================================================================
Entity* CWorld::createPlayer(vec2 _v2Pos) {
	Entity* player = NEW(Entity, Entity::EPlayer);
	ComponentTransform* transform = NEW(ComponentTransform, player, _v2Pos, vmake(30, 25));
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, player, "data/player.png", 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentPlayerController* playerControl = NEW(ComponentPlayerController, player);
	playerControl->init();
	ComponentMove* movement = NEW(ComponentMove, player, vmake(0.0f, 0.0f), m_fPlayerSpeed, false, false);
	movement->init();
	ComponentWeapon* weapon = NEW(ComponentWeapon, player, m_mWeaponData[ComponentWeapon::EEREVOLVER]);
	weapon->init();
	ComponentCollider* collider = NEW(ComponentCollider, player, ComponentCollider::ERectCollider, -1, ComponentCollider::EPlayerCollider, ComponentCollider::EEnemyCollider | ComponentCollider::EEnemyWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, player, m_iPlayerLife, 0, 20);
	life->init();
	ComponentHUD* hudComponent = NEW(ComponentHUD, player);
	hudComponent->init();
	addEntity(player);
	return player;
}

Entity* CWorld::createBullet(vec2 _v2Pos, vec2 _v2Size, vec2 _v2Direction, float _fSpeed, int _iDamage, int _iLife, int _iRange, bool _bIsExplossive, bool _bIsBouncy, Entity::TType _eEntityType, const char* _psTexture) {
	Entity* bullet = NEW(Entity, Entity::EWeapon);
	ComponentTransform* transform = NEW(ComponentTransform, bullet, _v2Pos, _v2Size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, bullet, _psTexture, vangle(_v2Direction), 1.0f, 5);
	renderable->init();
	ComponentMove* movement = NEW(ComponentMove, bullet, _v2Direction, _fSpeed, true, _bIsBouncy);
	movement->init();
	
	// Depending on the type of bullet it has different collider setup
	switch (_eEntityType) {
		case Entity::EPlayer: {
			ComponentCollider* collider = NEW(ComponentCollider, bullet, ComponentCollider::ECircleCollider, _iDamage, ComponentCollider::EPlayerWeaponCollider, ComponentCollider::EEnemyCollider | ComponentCollider::EBoundariesCollider);
			collider->init();
			break;
		}
		case Entity::EMine: {
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
	ComponentLife* componentLife = NEW(ComponentLife, bullet, _iLife, _iRange, 0);
	componentLife->init();
	addEntity(bullet);
	return bullet;
}

Entity* CWorld::createExplossion(vec2 _v2Pos, vec2 _v2Size, vec2 _v2SizeIncrement, int _iDuration, Entity::TType _eEntityType) {
	Entity* explossion = NEW(Entity, _eEntityType);
	ComponentTransform* transform = NEW(ComponentTransform, explossion, _v2Pos, _v2Size, _v2SizeIncrement);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, explossion, "data/explossion.png", 0.0f, 0.5f, 5);
	renderable->init();

	// Nuclear explossion has different collider than standard explosssion
	switch (_eEntityType) {
		case Entity::ENuclearExplossion: {
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
	ComponentLife* life = NEW(ComponentLife, explossion, 1, _iDuration, 0);
	life->init();
	g_pSoundEngine->playSound("data/explossion.wav");
	g_pWorld->addEntity(explossion);
	return explossion;
}

Entity* CWorld::createEnemy(vec2 _v2Pos, TEnemyData _tEnemyData, Entity* _pPlayer) {
	Entity* enemy = NEW(Entity, _tEnemyData.eType);
	ComponentTransform* transform = NEW(ComponentTransform, enemy, _v2Pos, _tEnemyData.v2Size);
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, enemy, _tEnemyData.sImageFile.c_str(), 0.0f, 1.0f, 5, 10);
	renderable->init();

	// Melee and Big enemies follow player until contact
	if (_tEnemyData.eType == Entity::EEnemyMelee || _tEnemyData.eType == Entity::EEnemyBig) {
		ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, _pPlayer, _tEnemyData.fSpeed, 0);
		aiMelee->init();
	}

	// Range and Turrets have a fire weapon
	if (_tEnemyData.eWeapon != ComponentWeapon::TType::EInvalid) {
		ComponentWeapon* gun = NEW(ComponentWeapon, enemy, m_mWeaponData[_tEnemyData.eWeapon]);
		gun->init();
		
		// If a player is passed the enemy keep a distance between ComponentAIMelee and ComponentAIEvade distances and aim to it
		if (_pPlayer && _tEnemyData.eType != Entity::EBoss) {
			ComponentAIFire* aiFire = NEW(ComponentAIFire, enemy, _pPlayer);
			aiFire->init();
			ComponentAIMelee* aiMelee = NEW(ComponentAIMelee, enemy, _pPlayer, _tEnemyData.fSpeed, 200);
			aiMelee->init();
			ComponentAIEvade* aiEvade = NEW(ComponentAIEvade, enemy, _pPlayer, _tEnemyData.fSpeed, 150);
			aiEvade->init();
		}
	}

	// Boss AI
	if (_tEnemyData.eType == Entity::EBoss) {
		BossIAComponent* bossAI = NEW(BossIAComponent, enemy, "data/bt/boss_bt.xml");
		bossAI->init();
	}

	ComponentCollider* collider = NEW(ComponentCollider, enemy, ComponentCollider::ERectCollider, _tEnemyData.iCollisionDamage, ComponentCollider::EEnemyCollider, ComponentCollider::EPlayerWeaponCollider);
	collider->init();
	ComponentLife* life = NEW(ComponentLife, enemy, _tEnemyData.iLife, 0, 0);
	life->init();
	ComponentPoints* points = NEW(ComponentPoints, enemy, _tEnemyData.iPoints);
	points->init();
	addEntity(enemy);
	return enemy;
}

Entity* CWorld::createEnemy(vec2 _v2Pos, TEnemyData _tEnemyData, vec2 _v2MoveDir, std::vector<vec2> _vAimDirections, bool _bIshuffleAim) {
	Entity* enemy = createEnemy(_v2Pos, _tEnemyData, nullptr);
	ComponentMove* movement = NEW(ComponentMove, enemy, _v2MoveDir, _tEnemyData.fSpeed, true, true);
	movement->init();

	// Used by the turrets to fire in the given directions and use a delay to not shoot all at the same time
	ComponentAIFire* aiFire = NEW(ComponentAIFire, enemy, _vAimDirections, _bIshuffleAim);
	aiFire->setActivationDelay(rand() % 100);
	aiFire->init();
	return enemy;
}

Entity* CWorld::createWeaponPickup() {
	// Calculate a random weapon type
	ComponentWeapon::TType type = static_cast<ComponentWeapon::TType>(rand() % ComponentWeapon::NUM_PLAYER_WEAPONS_TYPES);
	// Calculate a random spawn position
	vec2 randomPos = vmake(CORE_FRand(0.0, WORLD_WIDTH), CORE_FRand(80, WORLD_HEIGHT - 80));
	
	Entity* weaponPickup = NEW(Entity, Entity::EPickup);
	ComponentTransform* transform = NEW(ComponentTransform, weaponPickup, randomPos, vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = NEW(ComponentRenderable, weaponPickup, "data/crate-1.png", 0.0f, 1.0f, 5);
	renderable->init();
	ComponentCollider* collider = NEW(ComponentCollider, weaponPickup, ComponentCollider::ERectCollider, 0, ComponentCollider::EPickupCollider, ComponentCollider::EPlayerCollider);
	collider->init();
	ComponentWeaponPickup* pickup = NEW(ComponentWeaponPickup, weaponPickup, m_mWeaponData[type]);
	pickup->init();
	ComponentPoints* points = NEW(ComponentPoints, weaponPickup, m_iPickupPoints);
	points->init();
	ComponentLife* life = NEW(ComponentLife, weaponPickup, 1, 0, 0);
	life->init();
	addEntity(weaponPickup);
	return weaponPickup;
}

Entity* CWorld::createHUDMessage(const std::string& _sMessage, vec2 _v2Pos, int _iDisplayTime) {
	Entity* hudMessage = NEW(Entity, Entity::EHUDMessage);
	ComponentHUDMessage* hudMessageComponent = NEW(ComponentHUDMessage, hudMessage, _v2Pos, _sMessage);
	hudMessageComponent->init();
	ComponentLife* life = NEW(ComponentLife, hudMessage, 1, _iDisplayTime, 0);
	life->init();

	// Remove any previous HUD message still on screen
	if (m_pHudMessage) {
		g_pWorld->removeEntity(m_pHudMessage);
	}
	g_pWorld->addEntity(hudMessage);
	return hudMessage;
}
//=============================================================================

bool CWorld::loadConfig() {
	FILE* file = fopen("data/config.json", "r");
	if (!file) return false;

	fseek(file, 0, SEEK_END);
	std::vector<char> fileData(ftell(file));
	fseek(file, 0, SEEK_SET);
	FileReadStream is(file, fileData.data(), fileData.size());
	Document doc;
	doc.ParseStream(is);

	// Load weapons general configuration
	const Value& weapons = doc["weapons"];
	for (SizeType i = 0; i < weapons.Size(); i++) {
		ComponentWeapon::TWeaponData weapon;
		weapon.type               = ComponentWeapon::getWeaponTypeByName(weapons[i]["name"].GetString());
		weapon.fireRate           = weapons[i]["fireRate"].GetInt();
		weapon.reloadTime         = weapons[i]["reloadTime"].GetInt();
		weapon.capacity           = weapons[i]["capacity"].GetInt();
		weapon.bulletSpeed        = weapons[i]["bulletSpeed"].GetFloat();
		weapon.bulletDamage       = weapons[i]["bulletDamage"].GetInt();
		weapon.bulletLife         = weapons[i]["bulletLife"].GetInt();
		weapon.bulletRange        = weapons[i]["bulletRange"].GetInt();
		weapon.numBullets = 1;
		if (weapons[i].HasMember("numBullets")) {
			weapon.numBullets = weapons[i]["numBullets"].GetInt();
		}
		weapon.isAutomatic        = weapons[i]["isAutomatic"].GetBool();
		weapon.isExplossive       = weapons[i]["isExplossive"].GetBool();
		weapon.isBouncy           = weapons[i]["isBouncy"].GetBool();
		weapon.soundFile          = weapons[i]["soundFile"].GetString();
		m_mWeaponData[weapon.type] = weapon;
	}

	// Load enemies general configuration
	const Value& enemies = doc["enemies"];
	for (SizeType i = 0; i < enemies.Size(); i++) {
		TEnemyData enemy;
		enemy.eType              = static_cast<Entity::TType>(enemies[i]["id"].GetInt());
		enemy.iLife              = enemies[i]["life"].GetInt();
		enemy.fSpeed             = enemies[i]["speed"].GetFloat();
		enemy.iCollisionDamage   = enemies[i]["collisionDamage"].GetInt();
		enemy.eWeapon            = ComponentWeapon::TType::EInvalid;
		if (enemies[i].HasMember("weapon")) {
			enemy.eWeapon = static_cast<ComponentWeapon::TType>(enemies[i]["weapon"].GetInt());
		}
		enemy.v2Size              = vmake(enemies[i]["size"][0].GetFloat(), enemies[i]["size"][1].GetFloat());
		enemy.sImageFile          = enemies[i]["imageFile"].GetString();
		m_mEnemyData[enemy.eType] = enemy;
	}
	fclose(file);

	// Generate spawn points
	m_vSpawnData.push_back(vmake(WORLD_WIDTH / 2.0f, WORLD_HEIGHT));
	m_vSpawnData.push_back(vmake(WORLD_WIDTH, WORLD_HEIGHT / 2.0f));
	m_vSpawnData.push_back(vmake(0.0f, WORLD_HEIGHT / 2.0f));
	m_vSpawnData.push_back(vmake(WORLD_WIDTH / 2.0f, 0.0f));

	return true;
}

void CWorld::checkCollisions() {
	for (size_t i = 0; i < m_vEntities.size(); ++i) {
		Entity* entity1 = m_vEntities[i];
		for (size_t j = i + 1; j < m_vEntities.size(); ++j) {
			Entity* entity2 = m_vEntities[j];
			MessageCheckCollision msgCheckCollision;
			msgCheckCollision.other = entity2;
			entity1->receiveMessage(&msgCheckCollision);
		}
	}
}

void CWorld::removePendingEntities() {
	for (auto it = m_vEntitiesToRemove.begin(); it != m_vEntitiesToRemove.end(); ++it) {
		Entity::TType type = (*it)->getType();
		switch (type) {
			case Entity::EPlayer: {
				m_bIsGameOver = true;
				break;
			}
			case Entity::EPickup: {
				m_iPickupSpawnTimer = 0;
				break;
			}
			case Entity::EEnemyMelee:
			case Entity::EEnemyBig:
			case Entity::EEnemyRange:
			case Entity::ETurret:
			case Entity::EBoss: {
				--m_iCurrentEnemies;
				break;
			}
			case Entity::EHUDMessage: {
				m_pHudMessage = nullptr;
				break;
			}
		}
		auto it2 = m_vEntities.begin();
		bool bErased = false;
		while (!bErased && (it2 != m_vEntities.end())) {
			if (*it == *it2) {
				DELETE(*it2);
				m_vEntities.erase(it2);
				bErased = true;
			}
			else {
				++it2;
			}
		}
	}
	m_vEntitiesToRemove.clear();

	if (m_bIsGameOver) {
		g_pWorld->unloadLevel();
		std::string scoreMessage = g_pStringManager->getText("LTEXT_GUI_SCORE_MESSAGE") + std::to_string(m_uScore);
		g_pMenuManager->getMenu(MenuManager::EGameOverMenu)->setTitle(scoreMessage.c_str());
		g_pMenuManager->activateMenu(MenuManager::EGameOverMenu);
	}
}

void CWorld::addPendingEntities() {
	for (size_t i = 0; i < m_vEntitiesToAdd.size(); ++i) {
		m_vEntities.push_back(m_vEntitiesToAdd[i]);
		switch (m_vEntitiesToAdd[i]->getType()){
			case Entity::EPlayer: 
				m_pPlayer = m_vEntitiesToAdd[i];
				break;
			case Entity::EHUDMessage:
				m_pHudMessage = m_vEntitiesToAdd[i];
				break;
		}
	}
	m_vEntitiesToAdd.clear();
}

void CWorld::spawnNewEntities() {
	// Spawn Pickup
	if (m_iPickupSpawnTimer <= m_iPickupSpawnWait) {
		++m_iPickupSpawnTimer;
		if (m_iPickupSpawnTimer == m_iPickupSpawnWait) {
			createWeaponPickup();
		}
	}

	// Spawn Enemy
	++m_iEnemySpawnTimer;

	if (m_iEnemySpawnTimer >= m_iEnemySpawnWait && m_iCurrentEnemies < m_iMaxEnemies) {
		// Pick a random spawn point from the available ones
		vec2 spawnLocation = m_vSpawnData[rand() % m_vSpawnData.size()];

		// Pick a random enemy based on the configure probability of each one
		float enemyType = CORE_FRand(0.0f, 1.0f);
		for (auto itEnemyData = m_mEnemyData.begin(); itEnemyData != m_mEnemyData.end(); ++itEnemyData) {
			if (enemyType <= itEnemyData->second.fSpawnProbability) {
				createEnemy(spawnLocation, m_mEnemyData[itEnemyData->second.eType], m_pPlayer);
				break;
			}
		}
		++m_iCurrentEnemies;
		m_iEnemySpawnTimer = 0;
	}
}
