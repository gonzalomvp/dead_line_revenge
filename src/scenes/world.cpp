#include "../common/stdafx.h"
#include "../scenes/world.h"

#include "../engine/sound_engine.h"
#include "../entities/components/component.h"
#include "../entities/message.h"
#include "../gui/string_manager.h"
#include "../gui/menu.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

World::~World() {
	unloadLevel();
	if (g_inputManager) {
		g_inputManager->unregisterEvent(this, IInputManager::TEventType::EPause);
	}
}

void World::init() {
	g_inputManager->registerEvent(this, IInputManager::TEventType::EPause);
	loadConfig();
}

bool World::loadLevel() {
	m_player = nullptr;
	m_hudMessage = nullptr;
	m_isGameOver = false;
	m_isPaused = false;
	m_score = 0;
	m_currentEnemies = 0;
	m_pickupSpawnTimer = 0;
	m_enemySpawnTimer = 0;

	// Parse level file
	char* fileName = "";
	switch (m_level) {
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
	FILE* file = fopen(fileName, "r");
	if (!file) return false;

	fseek(file, 0, SEEK_END);
	std::vector<char> fileData(ftell(file));
	fseek(file, 0, SEEK_SET);
	FileReadStream is(file, fileData.data(), fileData.size());
	Document doc;
	doc.ParseStream(is);

	// Load general level rules
	m_playerLife       = doc["playerLife"].GetInt();
	m_playerSpeed      = doc["playerSpeed"].GetFloat();
	m_pickupPoints     = doc["pickupPoints"].GetInt();
	m_pickupSpawnWait  = doc["pickupSpawnWait"].GetInt();
	m_enemySpawnWait   = doc["enemySpawnWait"].GetInt();
	m_maxEnemies       = doc["maxEnemies"].GetInt();
	m_pickupSpawnTimer = m_pickupSpawnWait;

	// Load enemy level configuration
	float totalProbability = 0.0f;
	const Value& enemies   = doc["enemies"];
	for (SizeType i = 0; i < enemies.Size(); i++) {
		TEnemyData enemy        = m_enemyData[static_cast<Entity::TType>(enemies[i]["id"].GetInt())];
		totalProbability        += enemies[i]["spawnProbability"].GetFloat();
		enemy.spawnProbability  = totalProbability;
		enemy.points            = enemies[i]["points"].GetInt();
		m_enemyData[enemy.type] = enemy;
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
		createEnemy(pos, m_enemyData[Entity::ETurret], moveDirection, aimDirections, shuffleAim);
		++m_currentEnemies;
	}
	fclose(file);

	// Create player and first pickup
	createPlayer(vmake(WORLD_WIDTH * 0.5f, WORLD_HEIGHT * 0.5f));
	createWeaponPickup();

	return true;
}

void World::unloadLevel() {
	for (size_t i = 0; i < m_entities.size(); ++i) {
		delete m_entities[i];
	}
	m_entities.clear();

	for (size_t i = 0; i < m_entitiesToAdd.size(); ++i) {
		delete m_entitiesToAdd[i];
	}
	m_entitiesToAdd.clear();

	m_entitiesToRemove.clear();
}

void World::run(float deltaTime) {
	if (!m_isPaused && !m_isGameOver) {

		for (size_t i = 0; i < m_entities.size(); ++i) {
			m_entities[i]->run(deltaTime);
		}
		checkCollisions();
		removePendingEntities();
		spawnNewEntities();
		addPendingEntities();
	}
}

void World::addEntity(ptr<Entity> entity) {
	m_entitiesToAdd.push_back(entity);
}

void World::removeEntity(ptr<Entity> entity) {
	m_entitiesToRemove.push_back(entity);
}

bool World::onEvent(const IInputManager::Event& event) {
	if (m_isGameOver)
		return true;

	IInputManager::TEventType eventType = event.getType();
	if (eventType == IInputManager::TEventType::EPause) {
		m_isPaused = !m_isPaused;
		if (m_isPaused) {
			m_player->deactivate();
			g_menuManager->activateMenu(MenuManager::EPauseMenu);
		}
		else {
			m_player->activate();
			g_menuManager->deactivateMenu();
		}
	}
	return true;
}

//=============================================================================
// Entity creation methods
//=============================================================================
ptr<Entity> World::createPlayer(vec2 pos) {
	ptr<Entity> player = new Entity(Entity::EPlayer);
	ComponentTransform* transform = new ComponentTransform(player, pos, vmake(30, 25));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(player, "data/player.png", 0.0f, 1.0f, 5, 10);
	renderable->init();
	ComponentPlayerController* playerControl = new ComponentPlayerController(player);
	playerControl->init();
	ComponentMove* movement = new ComponentMove(player, vmake(0.0f, 0.0f), m_playerSpeed, false, false);
	movement->init();
	ComponentWeapon* weapon = new ComponentWeapon(player, m_weaponData[ComponentWeapon::ERevolver]);
	weapon->init();
	ComponentCollider* collider = new ComponentCollider(player, ComponentCollider::ERectCollider, -1, ComponentCollider::EPlayerCollider, ComponentCollider::EEnemyCollider | ComponentCollider::EEnemyWeaponCollider);
	collider->init();
	ComponentLife* life = new ComponentLife(player, 5, 0, 20);
	life->init();
	ComponentHUD* hudComponent = new ComponentHUD(player);
	hudComponent->init();
	addEntity(player);
	return player;
}

ptr<Entity> World::createBullet(vec2 pos, vec2 size, vec2 direction, float speed, int damage, int life, int range, bool isExplossive, bool isBouncy, Entity::TType entityType, const char* texture) {
	ptr<Entity> bullet = new Entity(Entity::EWeapon);
	ComponentTransform* transform = new ComponentTransform(bullet, pos, size);
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(bullet, texture, vangle(direction), 1.0f, 5);
	renderable->init();
	ComponentMove* movement = new ComponentMove(bullet, direction, speed, true, isBouncy);
	movement->init();
	
	// Depending on the type of bullet it has different collider setup
	switch (entityType) {
		case Entity::EPlayer: {
			ComponentCollider* collider = new ComponentCollider(bullet, ComponentCollider::ECircleCollider, damage, ComponentCollider::EPlayerWeaponCollider, ComponentCollider::EEnemyCollider | ComponentCollider::EBoundariesCollider);
			collider->init();
			break;
		}
		case Entity::EMine: {
			ComponentCollider* collider = new ComponentCollider(bullet, ComponentCollider::ECircleCollider, 0, ComponentCollider::ENoneCollider, ComponentCollider::EPlayerCollider | ComponentCollider::EEnemyCollider | ComponentCollider::EPlayerWeaponCollider | ComponentCollider::EEnemyWeaponCollider);
			collider->setActivationDelay(20);
			collider->init();
			break;
		}
		default: {
			ComponentCollider* collider = new ComponentCollider(bullet, ComponentCollider::ECircleCollider, damage, ComponentCollider::EEnemyWeaponCollider, ComponentCollider::EPlayerCollider | ComponentCollider::EBoundariesCollider);
			collider->init();
			break;
		}
	}
	if (isExplossive) {
		ComponentExplossive* explossive = new ComponentExplossive(bullet);
		explossive->init();
	}
	ComponentLife* componentLife = new ComponentLife(bullet, life, range, 0);
	componentLife->init();
	addEntity(bullet);
	return bullet;
}

ptr<Entity> World::createExplossion(vec2 pos, vec2 size, vec2 sizeIncrement, int duration, Entity::TType entityType) {
	ptr<Entity> explossion = new Entity(entityType);
	ComponentTransform* transform = new ComponentTransform(explossion, pos, size, sizeIncrement);
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(explossion, "data/explossion.png", 0.0f, 0.5f, 5);
	renderable->init();

	// Nuclear explossion has different collider than standard explosssion
	switch (entityType) {
		case Entity::ENuclearExplossion: {
			ComponentCollider* collider = new ComponentCollider(explossion, ComponentCollider::ECircleCollider, -50, ComponentCollider::EPlayerWeaponCollider | ComponentCollider::EBoundariesCollider, ComponentCollider::ENoneCollider);
			collider->init();
			break;
		}
		default: {
			ComponentCollider* collider = new ComponentCollider(explossion, ComponentCollider::ECircleCollider, -1, ComponentCollider::EPlayerWeaponCollider | ComponentCollider::EEnemyWeaponCollider | ComponentCollider::EBoundariesCollider, ComponentCollider::ENoneCollider);
			collider->init();
			break;
		}
	}
	ComponentLife* life = new ComponentLife(explossion, 1, duration, 0);
	life->init();
	g_soundEngine->playSound("data/explossion.wav");
	g_world->addEntity(explossion);
	return explossion;
}

ptr<Entity> World::createEnemy(vec2 pos, TEnemyData enemyData, ptr<Entity> player) {
	ptr<Entity> enemy = new Entity(enemyData.type);
	ComponentTransform* transform = new ComponentTransform(enemy, pos, enemyData.size);
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, enemyData.imageFile.c_str(), 0.0f, 1.0f, 5, 10);
	renderable->init();

	// Melee and Big enemies follow player until contact
	if (enemyData.type == Entity::EEnemyMelee || enemyData.type == Entity::EEnemyBig) {
		ComponentAIMelee* aiMelee = new ComponentAIMelee(enemy, player, enemyData.speed, 0);
		aiMelee->init();
	}

	// Range and Turrets have a fire weapon
	if (enemyData.type == Entity::EEnemyRange || enemyData.type == Entity::ETurret) {
		ComponentWeapon::TWeaponData weaponData;
		weaponData.fireRate     = enemyData.fireRate;
		weaponData.reloadTime   = 1;
		weaponData.capacity     = 1;
		weaponData.bulletSpeed  = enemyData.bulletSpeed;
		weaponData.bulletDamage = enemyData.bulletDamage;
		weaponData.bulletLife   = enemyData.bulletLife;
		weaponData.bulletRange  = enemyData.bulletRange;
		weaponData.isExplossive = enemyData.isExplossive;
		weaponData.isBouncy     = enemyData.isBouncy;
		weaponData.isAutomatic  = true;
		weaponData.soundFile    = "";
		ComponentWeapon* gun = new ComponentWeapon(enemy, weaponData);
		gun->init();

		// If a player is passed the enemy keep a distance between ComponentAIMelee and ComponentAIEvade distances and aim to it
		if (player) {
			ComponentAIFire* aiFire = new ComponentAIFire(enemy, player);
			aiFire->init();
			ComponentAIMelee* aiMelee = new ComponentAIMelee(enemy, player, enemyData.speed, 200);
			aiMelee->init();
			ComponentAIEvade* aiEvade = new ComponentAIEvade(enemy, player, enemyData.speed, 150);
			aiEvade->init();
		}
	}

	ComponentCollider* collider = new ComponentCollider(enemy, ComponentCollider::ERectCollider, enemyData.collisionDamage, ComponentCollider::EEnemyCollider, ComponentCollider::EPlayerWeaponCollider);
	collider->init();
	ComponentLife* life = new ComponentLife(enemy, enemyData.life, 0, 0);
	life->init();
	ComponentPoints* points = new ComponentPoints(enemy, enemyData.points);
	points->init();
	addEntity(enemy);
	return enemy;
}

ptr<Entity> World::createEnemy(vec2 pos, TEnemyData enemyData, vec2 moveDir, std::vector<vec2> aimDirections, bool shuffleAim) {
	ptr<Entity> enemy = createEnemy(pos, enemyData, nullptr);
	ComponentMove* movement = new ComponentMove(enemy, moveDir, enemyData.speed, true, true);
	movement->init();

	// Used by the turrets to fire in the given directions and use a delay to not shoot all at the same time
	ComponentAIFire* aiFire = new ComponentAIFire(enemy, aimDirections, shuffleAim);
	aiFire->setActivationDelay(rand() % 100);
	aiFire->init();
	return enemy;
}

ptr<Entity> World::createWeaponPickup() {
	// Calculate a random weapon type
	ComponentWeapon::TWeapon type = static_cast<ComponentWeapon::TWeapon>(rand() % ComponentWeapon::EWeaponCount);
	// Calculate a random spawn position
	vec2 randomPos = vmake(CORE_FRand(0.0, WORLD_WIDTH), CORE_FRand(80, WORLD_HEIGHT - 80));
	
	ptr<Entity> weaponPickup = new Entity(Entity::EPickup);
	ComponentTransform* transform = new ComponentTransform(weaponPickup, randomPos, vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(weaponPickup, "data/crate-1.png", 0.0f, 1.0f, 5);
	renderable->init();
	ComponentCollider* collider = new ComponentCollider(weaponPickup, ComponentCollider::ERectCollider, 0, ComponentCollider::EPickupCollider, ComponentCollider::EPlayerCollider);
	collider->init();
	ComponentWeaponPickup* pickup = new ComponentWeaponPickup(weaponPickup, m_weaponData[type]);
	pickup->init();
	ComponentPoints* points = new ComponentPoints(weaponPickup, m_pickupPoints);
	points->init();
	ComponentLife* life = new ComponentLife(weaponPickup, 1, 0, 0);
	life->init();
	addEntity(weaponPickup);
	return weaponPickup;
}

ptr<Entity> World::createHUDMessage(const std::string& message, vec2 pos, int displayTime) {
	ptr<Entity> hudMessage = new Entity(Entity::EHUDMessage);
	ptr<ComponentHUDMessage> hudMessageComponent = new ComponentHUDMessage(hudMessage, pos, message);
	hudMessageComponent->init();
	ComponentLife* life = new ComponentLife(hudMessage, 1, displayTime, 0);
	life->init();

	// Remove any previous HUD message still on screen
	if (m_hudMessage) {
		g_world->removeEntity(m_hudMessage);
	}
	g_world->addEntity(hudMessage);
	return hudMessage;
}
//=============================================================================

bool World::loadConfig() {
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
		weapon.type               = static_cast<ComponentWeapon::TWeapon>(weapons[i]["id"].GetInt());
		weapon.fireRate           = weapons[i]["fireRate"].GetInt();
		weapon.reloadTime         = weapons[i]["reloadTime"].GetInt();
		weapon.capacity           = weapons[i]["capacity"].GetInt();
		weapon.bulletSpeed        = weapons[i]["bulletSpeed"].GetFloat();
		weapon.bulletDamage       = weapons[i]["bulletDamage"].GetInt();
		weapon.bulletLife         = weapons[i]["bulletLife"].GetInt();
		weapon.bulletRange        = weapons[i]["bulletRange"].GetInt();
		weapon.isAutomatic        = weapons[i]["isAutomatic"].GetBool();
		weapon.isExplossive       = weapons[i]["isExplossive"].GetBool();
		weapon.isBouncy           = weapons[i]["isBouncy"].GetBool();
		weapon.soundFile          = weapons[i]["soundFile"].GetString();
		m_weaponData[weapon.type] = weapon;
	}

	// Load enemies general configuration
	const Value& enemies = doc["enemies"];
	for (SizeType i = 0; i < enemies.Size(); i++) {
		TEnemyData enemy;
		enemy.type              = static_cast<Entity::TType>(enemies[i]["id"].GetInt());
		enemy.life              = enemies[i]["life"].GetInt();
		enemy.speed             = enemies[i]["speed"].GetFloat();
		enemy.collisionDamage   = enemies[i]["collisionDamage"].GetInt();
		enemy.fireRate          = enemies[i]["fireRate"].GetInt();
		enemy.bulletSpeed       = enemies[i]["bulletSpeed"].GetFloat();
		enemy.bulletDamage      = enemies[i]["bulletDamage"].GetInt();
		enemy.bulletLife        = enemies[i]["bulletLife"].GetInt();
		enemy.bulletRange       = enemies[i]["bulletRange"].GetInt();
		enemy.isExplossive      = enemies[i]["isExplossive"].GetBool();
		enemy.isBouncy          = enemies[i]["isBouncy"].GetBool();
		enemy.size              = vmake(enemies[i]["size"][0].GetFloat(), enemies[i]["size"][1].GetFloat());
		enemy.imageFile         = enemies[i]["imageFile"].GetString();
		m_enemyData[enemy.type] = enemy;
	}
	fclose(file);

	// Generate spawn points
	m_spawnData.push_back(vmake(WORLD_WIDTH / 2.0f, WORLD_HEIGHT));
	m_spawnData.push_back(vmake(WORLD_WIDTH, WORLD_HEIGHT / 2.0f));
	m_spawnData.push_back(vmake(0.0f, WORLD_HEIGHT / 2.0f));
	m_spawnData.push_back(vmake(WORLD_WIDTH / 2.0f, 0.0f));

	return true;
}

void World::checkCollisions() {
	for (size_t i = 0; i < m_entities.size(); ++i) {
		ptr<Entity> entity1 = m_entities[i];
		for (size_t j = i + 1; j < m_entities.size(); ++j) {
			ptr<Entity> entity2 = m_entities[j];
			MessageCheckCollision msgCheckCollision;
			msgCheckCollision.other = entity2;
			entity1->receiveMessage(&msgCheckCollision);
		}
	}
}

void World::removePendingEntities() {
	for (auto it = m_entitiesToRemove.begin(); it != m_entitiesToRemove.end(); ++it) {
		Entity::TType type = (*it)->getType();
		switch (type) {
			case Entity::EPlayer: {
				m_isGameOver = true;
				break;
			}
			case Entity::EPickup: {
				m_pickupSpawnTimer = 0;
				break;
			}
			case Entity::EEnemyMelee:
			case Entity::EEnemyBig:
			case Entity::EEnemyRange:
			case Entity::ETurret: {
				--m_currentEnemies;
				break;
			}
			case Entity::EHUDMessage: {
				m_hudMessage = nullptr;
				break;
			}
		}
		auto it2 = m_entities.begin();
		bool bErased = false;
		while (!bErased && (it2 != m_entities.end())) {
			if (*it == *it2) {
				delete *it2;
				m_entities.erase(it2);
				bErased = true;
			}
			else {
				++it2;
			}
		}
	}
	m_entitiesToRemove.clear();

	if (m_isGameOver) {
		g_world->unloadLevel();
		std::string scoreMessage = g_stringManager->getText("LTEXT_GUI_SCORE_MESSAGE") + std::to_string(m_score);
		g_menuManager->getMenu(MenuManager::EGameOverMenu)->setTitle(scoreMessage.c_str());
		g_menuManager->activateMenu(MenuManager::EGameOverMenu);
	}
}

void World::addPendingEntities() {
	for (size_t i = 0; i < m_entitiesToAdd.size(); ++i) {
		m_entities.push_back(m_entitiesToAdd[i]);
		switch (m_entitiesToAdd[i]->getType()){
			case Entity::EPlayer: 
				m_player = m_entitiesToAdd[i];
				break;
			case Entity::EHUDMessage:
				m_hudMessage = m_entitiesToAdd[i];
				break;
		}
	}
	m_entitiesToAdd.clear();
}

void World::spawnNewEntities() {
	// Spawn Pickup
	if (m_pickupSpawnTimer <= m_pickupSpawnWait) {
		++m_pickupSpawnTimer;
		if (m_pickupSpawnTimer == m_pickupSpawnWait) {
			createWeaponPickup();
		}
	}

	// Spawn Enemy
	++m_enemySpawnTimer;

	if (m_enemySpawnTimer >= m_enemySpawnWait && m_currentEnemies < m_maxEnemies) {
		// Pick a random spawn point from the available ones
		vec2 spawnLocation = m_spawnData[rand() % m_spawnData.size()];

		// Pick a random enemy based on the configure probability of each one
		float enemyType = CORE_FRand(0.0f, 1.0f);
		for (auto itEnemyData = m_enemyData.begin(); itEnemyData != m_enemyData.end(); ++itEnemyData) {
			if (enemyType <= itEnemyData->second.spawnProbability) {
				createEnemy(spawnLocation, m_enemyData[itEnemyData->second.type], m_player);
				break;
			}
		}
		++m_currentEnemies;
		m_enemySpawnTimer = 0;
	}
}
