#include "common/stdafx.h"
#include "scenes/world.h"

#include "engine/sound_engine.h"
#include "entities/entities_factory.h"
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
	//fileName = "data/levelTest.json";

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
	m_iPickupSpawnWait  = doc["pickupSpawnWait"].GetInt();
	m_iEnemySpawnWait   = doc["enemySpawnWait"].GetInt();
	m_iMaxEnemies       = doc["maxEnemies"].GetInt();
	m_iPickupSpawnTimer = m_iPickupSpawnWait;
	m_mEntityPoints[Entity::EPickup] = doc["pickupPoints"].GetInt();

	// Load enemy level configuration
	float totalProbability = 0.0f;
	const Value& enemies   = doc["enemies"];
	for (SizeType i = 0; i < enemies.Size(); i++) {
		Entity::TType enemyType = static_cast<Entity::TType>(enemies[i]["id"].GetInt());
		totalProbability          += enemies[i]["spawnProbability"].GetFloat();
		m_mEnemyProbability[enemyType] = totalProbability;
		m_mEntityPoints[enemyType]      = enemies[i]["points"].GetInt();
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
		addEntity(g_pEntitiesFactory->createEnemy(pos, Entity::ETurret, moveDirection, aimDirections, shuffleAim));
		++m_iCurrentEnemies;
	}
	fclose(file);

	// Create player and first pickup
	addEntity(g_pEntitiesFactory->createPlayer(vmake(WORLD_WIDTH * 0.5f, WORLD_HEIGHT * 0.5f)));
	addEntity(g_pEntitiesFactory->createWeaponPickup());

	// Generate spawn points
	m_vSpawnPositions.push_back(vmake(WORLD_WIDTH / 2.0f, WORLD_HEIGHT));
	m_vSpawnPositions.push_back(vmake(WORLD_WIDTH, WORLD_HEIGHT / 2.0f));
	m_vSpawnPositions.push_back(vmake(0.0f, WORLD_HEIGHT / 2.0f));
	m_vSpawnPositions.push_back(vmake(WORLD_WIDTH / 2.0f, 0.0f));

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
				if (m_pHudMessage == *it) {
					m_pHudMessage = nullptr;
				}
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
				// Remove any previous HUD message still on screen
				if (m_pHudMessage) {
					g_pWorld->removeEntity(m_pHudMessage);
				}
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
			addEntity(g_pEntitiesFactory->createWeaponPickup());
		}
	}

	// Spawn Enemy
	++m_iEnemySpawnTimer;

	if (m_iEnemySpawnTimer >= m_iEnemySpawnWait && m_iCurrentEnemies < m_iMaxEnemies) {
		// Pick a random spawn point from the available ones
		vec2 spawnLocation = m_vSpawnPositions[rand() % m_vSpawnPositions.size()];

		// Pick a random enemy based on the configure probability of each one
		float enemyType = CORE_FRand(0.0f, 1.0f);
		for (auto itEnemyProbability = m_mEnemyProbability.begin(); itEnemyProbability != m_mEnemyProbability.end(); ++itEnemyProbability) {
			if (enemyType <= itEnemyProbability->second) {
				addEntity(g_pEntitiesFactory->createEnemy(spawnLocation, itEnemyProbability->first, m_pPlayer));
				break;
			}
		}
		
		++m_iCurrentEnemies;
		m_iEnemySpawnTimer = 0;
	}
}
