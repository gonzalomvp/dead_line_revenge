#include "../common/stdafx.h"
#include "../scenes/world.h"

#include "../engine/sound_engine.h"
#include "../entities/entity.h"
#include "../entities/components/component.h"
#include "../entities/message.h"
#include "../gui/string_manager.h"
#include "../gui/menu.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

World::~World() {
	destroy();
}

void World::init() {
	m_player           = nullptr;
	m_hudMessage       = nullptr;
	m_isGameOver       = false;
	m_isPaused         = false;
	m_score            = 0;
	m_currentEnemies   = 0;
	m_pickupSpawnTimer = 0;
	m_enemySpawnTimer  = 0;

	g_inputManager->registerEvent(this, IInputManager::TEvent::EPause, 0);

	// Create player and first pickup
	createPlayer(vmake(WORLD_WIDTH * 0.5f, WORLD_HEIGHT * 0.5f));
	addEntity(createWeaponPickup());

	// Load level details from file
	char* fileName = nullptr;
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
	loadLevel(fileName);

	//Cargar de fichero
	//addEntity(Entity::createTurretEnemy(100, SCR_HEIGHT - 100, vmake(0, -1), m_player));
	//addEntity(Entity::createTurretEnemy(SCR_WIDTH - 100, 100, vmake(-1, 0), m_player));	
}

void World::destroy() {
	for (size_t i = 0; i < m_entities.size(); ++i) {
		delete m_entities[i];
	}
	m_entities.clear();
	m_entitiesToRemove.clear();

	for (size_t i = 0; i < m_entitiesToAdd.size(); ++i) {
		delete m_entitiesToAdd[i];
	}
	m_entitiesToAdd.clear();

	g_inputManager->unregisterEvent(this, IInputManager::TEvent::EPause);
}

void World::addEntity(Entity* entity) {
	m_entitiesToAdd.push_back(entity);
}

void World::removeEntity(Entity* entity) {
	m_entitiesToRemove.push_back(entity);
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

void World::checkCollisions() {
	for (size_t i = 0; i < m_entities.size(); ++i) {
		Entity* entityCollision = m_entities[i];
		for (size_t j = i + 1; j < m_entities.size(); ++j) {
			Entity* entityToCheck = m_entities[j];
			MessageCheckCollision msgCheckCollision;
			msgCheckCollision.other = entityToCheck;
			entityCollision->receiveMessage(&msgCheckCollision);
		}
	}
}

void World::removePendingEntities() {
	for (auto it = m_entitiesToRemove.begin(); it != m_entitiesToRemove.end(); ++it)
	{
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
			case Entity::EHUDMessage:
				m_hudMessage = nullptr;
				break;
		}
		auto it2 = m_entities.begin();
		bool bErased = false;
		while (!bErased && (it2 != m_entities.end()))
		{
			if (*it == *it2)
			{
				delete *it2;
				m_entities.erase(it2);
				bErased = true;
			}
			else
				++it2;
		}
	}
	m_entitiesToRemove.clear();
	if (m_isGameOver)
	{
		g_world->destroy();
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

bool checkCircleCircle(const vec2& pos1, float radius1, const vec2& pos2, float radius2) {
	return vdist2(pos1, pos2) < (radius1 + radius2) * (radius1 + radius2);
}

bool checkCircleRect(const vec2& circlePos, float circleRadius, const vec2& rectPos, const vec2& rectSize) {
	vec2 closestPoint = vmake(clamp(circlePos.x, rectPos.x, rectPos.x + rectSize.x), clamp(circlePos.y, rectPos.y, rectPos.y + rectSize.y));
	return vdist(circlePos, closestPoint) < circleRadius;
}

bool checkRectRect(const vec2& rectPos1, const vec2& rectSize1, const vec2& rectPos2, const vec2& rectSize2) {
	return ((isInBounds(rectPos1.x, rectPos2.x, rectPos2.x + rectSize2.x) || isInBounds(rectPos2.x, rectPos1.x, rectPos1.x + rectSize1.x) || rectPos1.x == rectPos2.x) &&
		(isInBounds(rectPos1.y, rectPos2.y, rectPos2.y + rectSize2.y) || isInBounds(rectPos2.y, rectPos1.y, rectPos1.y + rectSize1.y) || rectPos1.y == rectPos2.y));
}

bool World::onEvent(const IInputManager::Event& event) {
	IInputManager::TEvent eventType = event.getType();
	if (eventType == IInputManager::TEvent::EPause) {
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

bool World::loadLevel(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (!file) return false;

	fseek(file, 0, SEEK_END);
	std::vector<char> fileData(ftell(file));
	fseek(file, 0, SEEK_SET);
	FileReadStream is(file, fileData.data(), fileData.size());
	Document doc;
	doc.ParseStream(is);

	m_playerLife = doc["playerLife"].GetInt();
	m_pickupSpawnWait = doc["pickupSpawnWait"].GetInt();
	m_pickupSpawnTimer = m_pickupSpawnWait;
	m_enemySpawnWait = doc["enemySpawnWait"].GetInt();
	m_maxEnemies = doc["maxEnemies"].GetInt();

	float totalProbability = 0.0f;
	const Value& enemies = doc["enemies"];
	assert(enemies.IsArray());
	for (SizeType i = 0; i < enemies.Size(); i++) {
		TEnemyData enemy = m_enemyData[static_cast<Entity::TType>(enemies[i]["id"].GetInt())];
		if (enemies[i].HasMember("spawnProbability")) {
			totalProbability += enemies[i]["spawnProbability"].GetFloat();
			enemy.spawnProbability = totalProbability;
		if (enemies[i].HasMember("points"))
			enemy.points = enemies[i]["points"].GetInt();
		}
		m_enemyData[enemy.type] = enemy;
	}

	const Value& turrets = doc["turrets"];
	assert(turrets.IsArray());
	for (SizeType i = 0; i < turrets.Size(); i++) {
		vec2 pos = vmake(turrets[i]["position"][0].GetInt(), turrets[i]["position"][1].GetInt());
		vec2 moveDirection = vmake(turrets[i]["moveDirection"][0].GetInt(), turrets[i]["moveDirection"][1].GetInt());
		
		std::vector<vec2> aimDirections;
		const Value& directions = turrets[i]["aimDirections"];
		assert(directions.IsArray());
		for (SizeType j= 0; j < directions.Size(); j++) {
			aimDirections.push_back(vmake(directions[j][0].GetInt(), directions[j][1].GetInt()));
		}
		bool shuffleAim = turrets[i]["shuffleAim"].GetBool();

		createEnemy(pos, m_enemyData[Entity::ETurret], moveDirection, aimDirections, shuffleAim);
		++m_currentEnemies;
	}

	fclose(file);

	return true;
}

bool World::loadConfig() {
	FILE* file = fopen("data/config.json", "r");
	if (!file) return false;

	fseek(file, 0, SEEK_END);
	std::vector<char> fileData(ftell(file));
	fseek(file, 0, SEEK_SET);
	FileReadStream is(file, fileData.data(), fileData.size());
	Document doc;
	doc.ParseStream(is);

	const Value& weapons = doc["weapons"];
	assert(weapons.IsArray());
	for (SizeType i = 0; i < weapons.Size(); i++) {
		Component::TWeaponData weapon;
		weapon.type = static_cast<Component::TWeapon>(weapons[i]["id"].GetInt());
		if (weapons[i].HasMember("fireRate"))
			weapon.fireRate = weapons[i]["fireRate"].GetInt();
		if (weapons[i].HasMember("reloadTime"))
			weapon.reloadTime = weapons[i]["reloadTime"].GetInt();
		if (weapons[i].HasMember("capacity"))
			weapon.capacity = weapons[i]["capacity"].GetInt();
		if (weapons[i].HasMember("bulletSpeed"))
			weapon.bulletSpeed = weapons[i]["bulletSpeed"].GetInt();
		if (weapons[i].HasMember("bulletDamage"))
			weapon.bulletDamage = weapons[i]["bulletDamage"].GetInt();
		if (weapons[i].HasMember("bulletLife"))
			weapon.bulletLife = weapons[i]["bulletLife"].GetInt();
		if (weapons[i].HasMember("bulletRange"))
			weapon.bulletRange = weapons[i]["bulletRange"].GetInt();
		if (weapons[i].HasMember("isAutomatic"))
			weapon.isAutomatic = weapons[i]["isAutomatic"].GetBool();
		if (weapons[i].HasMember("isExplossive"))
			weapon.isExplossive = weapons[i]["isExplossive"].GetBool();
		if (weapons[i].HasMember("isBouncy"))
			weapon.isBouncy = weapons[i]["isBouncy"].GetBool();
		m_weaponData[weapon.type] = weapon;
	}

	const Value& enemies = doc["enemies"];
	assert(enemies.IsArray());
	for (SizeType i = 0; i < enemies.Size(); i++) {
		TEnemyData enemy;
		enemy.type = static_cast<Entity::TType>(enemies[i]["id"].GetInt());
		if (enemies[i].HasMember("life"))
			enemy.life = enemies[i]["life"].GetInt();
		if (enemies[i].HasMember("speed"))
			enemy.speed = enemies[i]["speed"].GetInt();
		if (enemies[i].HasMember("collisionDamage"))
			enemy.collisionDamage = enemies[i]["collisionDamage"].GetInt();
		if (enemies[i].HasMember("fireRate"))
			enemy.fireRate = enemies[i]["fireRate"].GetInt();
		if (enemies[i].HasMember("bulletSpeed"))
			enemy.bulletSpeed = enemies[i]["bulletSpeed"].GetInt();
		if (enemies[i].HasMember("bulletDamage"))
			enemy.bulletDamage = enemies[i]["bulletDamage"].GetInt();
		if (enemies[i].HasMember("bulletLife"))
			enemy.bulletLife = enemies[i]["bulletLife"].GetInt();
		if (enemies[i].HasMember("bulletRange"))
			enemy.bulletRange = enemies[i]["bulletRange"].GetInt();
		if (enemies[i].HasMember("isExplossive"))
			enemy.isExplossive = enemies[i]["isExplossive"].GetBool();
		if (enemies[i].HasMember("isBouncy"))
			enemy.isBouncy = enemies[i]["isBouncy"].GetBool();
		if (enemies[i].HasMember("size"))
			enemy.size = vmake(enemies[i]["size"][0].GetInt(), enemies[i]["size"][1].GetInt());
		if (enemies[i].HasMember("imageFile"))
			enemy.imageFile = enemies[i]["imageFile"].GetString();
		m_enemyData[enemy.type] = enemy;
	}

	fclose(file);

	m_spawnData.push_back(vmake(WORLD_WIDTH / 2.0f, WORLD_HEIGHT));
	m_spawnData.push_back(vmake(WORLD_WIDTH, WORLD_HEIGHT / 2.0f));
	m_spawnData.push_back(vmake(0.0f, WORLD_HEIGHT / 2.0f));
	m_spawnData.push_back(vmake(WORLD_WIDTH / 2.0f, 0.0f));

	return true;
}

void World::spawnEnemy() {
	vec2 spawnLocation = m_spawnData[rand() % m_spawnData.size()];
	float enemyType = CORE_FRand(0.0f, 1.0f);
	Entity* enemy = nullptr;
	for (auto itEnemyData = m_enemyData.begin(); itEnemyData != m_enemyData.end(); ++itEnemyData) {
		if (enemyType <= itEnemyData->second.spawnProbability) {
			createEnemy(spawnLocation, m_enemyData[itEnemyData->second.type], m_player);
			break;
		}
	}
	++m_currentEnemies;
}

void World::spawnNewEntities() {
	//Pick up
	if (m_pickupSpawnTimer <= m_pickupSpawnWait) {
		++m_pickupSpawnTimer;
		if (m_pickupSpawnTimer == m_pickupSpawnWait) {
			addEntity(createWeaponPickup());
		}
	}

	//enemy
	++m_enemySpawnTimer;

	if (m_enemySpawnTimer >= m_enemySpawnWait && m_currentEnemies < m_maxEnemies) {
		spawnEnemy();
		m_enemySpawnTimer = 0;
	}
}

Component::TWeaponData World::getWeaponData(Component::TWeapon weaponType) {
	return m_weaponData[weaponType];
}

Entity* World::createPlayer(vec2 pos) {
	Entity* player = new Entity(Entity::EPlayer);
	ComponentTransform* transform = new ComponentTransform(player, pos, vmake(30, 25));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(player, "data/player.png", 0.0f, 1.0f, 2, 10);
	renderable->init();
	ComponentPlayerController* playerControl = new ComponentPlayerController(player);
	playerControl->init();
	ComponentMove* movement = new ComponentMove(player, vmake(0.0f, 0.0f), m_playerSpeed, false, false);
	movement->init();
	ComponentWeapon* weapon = new ComponentWeapon(player, m_weaponData[Component::ERevolver]);
	weapon->init();
	ComponentCollider* collider = new ComponentCollider(player, ComponentCollider::ERectCollider, -1, ComponentCollider::EPlayer, ComponentCollider::EEnemyC | ComponentCollider::EEnemyWeapon);
	collider->init();
	ComponentLife* life = new ComponentLife(player, 5, 0, 20);
	life->init();
	ComponentHUD* hudComponent = new ComponentHUD(player);
	hudComponent->init();
	addEntity(player);
	return player;
}

Entity* World::createBullet(vec2 pos, vec2 size, vec2 direction, float speed, int damage, int life, int range, bool isExplossive, bool isBouncy, Entity::TType entityType, const char* texture) {
	Entity* bullet = new Entity(Entity::EWeapon);
	ComponentTransform* transform = new ComponentTransform(bullet, pos, size);
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(bullet, texture, vangle(direction), 1.0f, 2);
	renderable->init();
	ComponentMove* movement = new ComponentMove(bullet, direction, speed, true, isBouncy);
	movement->init();
	switch (entityType) {
		case Entity::EPlayer: {
			ComponentCollider* collider = new ComponentCollider(bullet, ComponentCollider::ECircleCollider, damage, ComponentCollider::EPlayerWeapon, ComponentCollider::EEnemyC | ComponentCollider::EBoundaries);
			collider->init();
			break;
		}
		case Entity::EMine: {
			ComponentCollider* collider = new ComponentCollider(bullet, ComponentCollider::ECircleCollider, 0, ComponentCollider::ENone, ComponentCollider::EPlayer | ComponentCollider::EEnemyC | ComponentCollider::EPlayerWeapon | ComponentCollider::EEnemyWeapon);
			collider->setActivationDelay(20);
			collider->init();
			break;
		}
		default: {
			ComponentCollider* collider = new ComponentCollider(bullet, ComponentCollider::ECircleCollider, damage, ComponentCollider::EEnemyWeapon, ComponentCollider::EPlayer | ComponentCollider::EBoundaries);
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

Entity* World::createExplossion(vec2 pos, vec2 size, vec2 sizeIncrement, int duration, Entity::TType entityType) {
	Entity* explossion = new Entity(entityType);
	ComponentTransform* transform = new ComponentTransform(explossion, pos, size, sizeIncrement);
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(explossion, "data/explossion2.png", 0.0f, 0.5f, 2);
	renderable->init();
	switch (entityType) {
		case Entity::ENuclearExplossion: {
			ComponentCollider* collider = new ComponentCollider(explossion, ComponentCollider::ECircleCollider, -50, ComponentCollider::EPlayerWeapon | ComponentCollider::EBoundaries, ComponentCollider::ENone);
			collider->init();
			break;
		}
		default: {
			ComponentCollider* collider = new ComponentCollider(explossion, ComponentCollider::ECircleCollider, -1, ComponentCollider::EPlayerWeapon | ComponentCollider::EEnemyWeapon | ComponentCollider::EBoundaries, ComponentCollider::ENone);
			collider->init();
			break;
		}
	}
	ComponentLife* life = new ComponentLife(explossion, 1, duration, 0);
	life->init();
	g_world->addEntity(explossion);

	g_soundEngine->playSound("data/explossion.wav");
	return explossion;
}

Entity* World::createEnemy(vec2 pos, TEnemyData enemyData, Entity* player) {
	Entity* enemy = new Entity(enemyData.type);
	ComponentTransform* transform = new ComponentTransform(enemy, pos, enemyData.size);
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, enemyData.imageFile.c_str(), 0.0f, 1.0f, 2, 10);
	renderable->init();
	
	if (enemyData.type == Entity::EEnemyMelee || enemyData.type == Entity::EEnemyBig) {
		ComponentAIMelee* aiMelee = new ComponentAIMelee(enemy, player, enemyData.speed, 0);
		aiMelee->init();
	}

	if (enemyData.type == Entity::EEnemyRange || enemyData.type == Entity::ETurret) {
		Component::TWeaponData weaponData;
		weaponData.fireRate = enemyData.fireRate;
		weaponData.reloadTime = 1;
		weaponData.capacity = 1;
		weaponData.bulletSpeed = enemyData.bulletSpeed;
		weaponData.bulletDamage = enemyData.bulletDamage;
		weaponData.bulletLife = enemyData.bulletLife;
		weaponData.bulletRange = enemyData.bulletRange;
		weaponData.isExplossive = enemyData.isExplossive;
		weaponData.isBouncy = enemyData.isBouncy;
		weaponData.isAutomatic = true;
		ComponentWeapon* gun = new ComponentWeapon(enemy, weaponData);
		gun->init();
		if (player) {
			ComponentAIFire* aiFire = new ComponentAIFire(enemy, player);
			aiFire->init();
			ComponentAIMelee* aiMelee = new ComponentAIMelee(enemy, player, enemyData.speed, 200);
			aiMelee->init();
			ComponentAIEvade* aiLong = new ComponentAIEvade(enemy, player, enemyData.speed, 150);
			aiLong->init();
		}
	}

	ComponentCollider* collider = new ComponentCollider(enemy, ComponentCollider::ERectCollider, enemyData.collisionDamage, ComponentCollider::EEnemyC, ComponentCollider::EPlayerWeapon);
	collider->init();
	ComponentLife* life = new ComponentLife(enemy, enemyData.life, 0, 0);
	life->init();
	ComponentPoints* points = new ComponentPoints(enemy, enemyData.points);
	points->init();
	addEntity(enemy);
	return enemy;
}

Entity* World::createEnemy(vec2 pos, TEnemyData enemyData, vec2 moveDir, std::vector<vec2> aimDirections, bool shuffleAim) {
	Entity* enemy = createEnemy(pos, enemyData, nullptr);
	ComponentMove* movement = new ComponentMove(enemy, moveDir, enemyData.speed, true, true);
	movement->init();
	ComponentAIFire* aiFire = new ComponentAIFire(enemy, aimDirections, shuffleAim);
	aiFire->setActivationDelay(rand() % 100);
	aiFire->init();
	return enemy;
}

Entity* World::createWeaponPickup() {
	Component::TWeapon type = static_cast<Component::TWeapon>(rand() % Component::EWeaponCount);
	//type = Component::EMines;
	vec2 randomPos = vmake(CORE_FRand(0.0, WORLD_WIDTH), CORE_FRand(80, WORLD_HEIGHT - 80));
	Entity* weaponPickup = new Entity(Entity::EPickup);
	ComponentTransform* transform = new ComponentTransform(weaponPickup, randomPos, vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(weaponPickup, "data/crate-1.png", 0.0f, 1.0f, 2);
	renderable->init();
	ComponentCollider* collider = new ComponentCollider(weaponPickup, ComponentCollider::ERectCollider, 0, ComponentCollider::EPickup, ComponentCollider::EPlayer);
	collider->init();
	ComponentWeaponPickup* pickup = new ComponentWeaponPickup(weaponPickup, type);
	pickup->init();
	ComponentPoints* points = new ComponentPoints(weaponPickup, 10);
	points->init();
	ComponentLife* life = new ComponentLife(weaponPickup, 1, 0, 0);
	life->init();
	return weaponPickup;
}

Entity* World::createHUDMessage(std::string message, vec2 pos, int displayTime) {
	Entity* hudMessage = new Entity(Entity::EHUDMessage);
	ComponentHUDMessage* hudMessageComponent = new ComponentHUDMessage(hudMessage, pos, message);
	hudMessageComponent->init();
	ComponentLife* life = new ComponentLife(hudMessage, 1, displayTime, 0);
	life->init();
	if (g_world->getHUDMessage()) {
		g_world->removeEntity(g_world->getHUDMessage());
	}
	g_world->addEntity(hudMessage);
	return hudMessage;
}

