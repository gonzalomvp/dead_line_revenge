#include "common/stdafx.h"
#include "scenes/world.h"

#include "entities/entities_factory.h"
#include "entities/message.h"
#include "gui/menu.h"
#include "gui/string_manager.h"

#include <algorithm>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

CWorld::CWorld() 
: m_uLevel(0)
, m_pPlayer(nullptr)
, m_pHudMessage(nullptr)
, m_bIsGameOver(false)
, m_bIsPaused(false)
, m_uScore(0)
, m_iPlayerLife(0)
, m_fPlayerSpeed(0.0f)
, m_iPickupSpawnWait(0)
, m_iEnemySpawnWait(0)
, m_iCurrentEnemies(0)
, m_iMaxEnemies(0)
, m_iPickupSpawnTimer(0)
, m_iEnemySpawnTimer(0)
{}

CWorld::~CWorld() {
	cleanup();
}

bool CWorld::init(uint16_t _uLevel) {
	m_uLevel = _uLevel;
	m_pPlayer = nullptr;
	m_pHudMessage = nullptr;
	m_bIsGameOver = false;
	m_bIsPaused = false;
	m_uScore = 0;
	m_iPlayerLife = 0;
	m_fPlayerSpeed = 0.0f;
	m_iPickupSpawnWait = 0;
	m_iEnemySpawnWait = 0;
	m_iCurrentEnemies = 0;
	m_iMaxEnemies = 0;
	m_iPickupSpawnTimer = 0;
	m_iEnemySpawnTimer = 0;

	m_vEntities.clear();
	m_vEntitiesToRemove.clear();
	m_vEntitiesToAdd.clear();

	m_mEnemyProbabilities.clear();
	m_mEntityPoints.clear();
	m_vSpawnPositions.clear();

	ASSERT(g_pInputManager);
	g_pInputManager->registerEvent(this, IInputManager::TEventType::EPause);

	// Parse level file
	const char* psFileName = "";
	switch (_uLevel) {
	case 1:
		psFileName = "data/level1.json";
		break;
	case 2:
		psFileName = "data/level2.json";
		break;
	case 3:
		psFileName = "data/level3.json";
		break;
	default:
		// Invalid level
		return false;
	}

	// Force test level
	//fileName = "data/levelTest.json";

	FILE* pFile = fopen(psFileName, "r");
	if (!pFile) return false;

	fseek(pFile, 0, SEEK_END);
	std::vector<char> vFileData(ftell(pFile));
	fseek(pFile, 0, SEEK_SET);
	FileReadStream is(pFile, vFileData.data(), vFileData.size());
	Document doc;
	doc.ParseStream(is);

	// Load general level rules
	ASSERT(doc.HasMember("playerLife"));
	m_iPlayerLife = doc["playerLife"].GetInt();
	ASSERT(doc.HasMember("playerSpeed"));
	m_fPlayerSpeed = doc["playerSpeed"].GetFloat();
	ASSERT(doc.HasMember("pickupSpawnWait"));
	m_iPickupSpawnWait = doc["pickupSpawnWait"].GetInt();
	m_iPickupSpawnTimer = m_iPickupSpawnWait;
	ASSERT(doc.HasMember("enemySpawnWait"));
	m_iEnemySpawnWait = doc["enemySpawnWait"].GetInt();
	ASSERT(doc.HasMember("maxEnemies"));
	m_iMaxEnemies = doc["maxEnemies"].GetInt();
	ASSERT(doc.HasMember("pickupPoints"));
	m_mEntityPoints[Entity::EPICKUP] = doc["pickupPoints"].GetInt();

	// Load enemy level configuration
	float fTotalProbability = 0.0f;
	ASSERT(doc.HasMember("enemies"));
	const Value& enemies = doc["enemies"];
	for (SizeType i = 0; i < enemies.Size(); i++) {
		ASSERT(enemies[i].HasMember("type"));
		Entity::TType eEnemyType = CEntitiesFactory::getEntityTypeByName(enemies[i]["type"].GetString());
		ASSERT(enemies[i].HasMember("spawnProbability"));
		fTotalProbability += enemies[i]["spawnProbability"].GetFloat();
		m_mEnemyProbabilities[eEnemyType] = fTotalProbability;
		ASSERT(enemies[i].HasMember("points"));
		m_mEntityPoints[eEnemyType] = enemies[i]["points"].GetInt();
	}

	// Load turret level configuration
	const Value& turrets = doc["turrets"];
	for (SizeType i = 0; i < turrets.Size(); i++) {
		std::vector<vec2> vAimDirections;
		ASSERT(turrets[i].HasMember("position") && turrets[i]["position"].Size() == 2);
		vec2 v2Pos = vmake(turrets[i]["position"][0].GetFloat(), turrets[i]["position"][1].GetFloat());

		vec2 v2MoveDirection = vmake(0.0f, 0.0f);
		if (turrets[i].HasMember("moveDirection")) {
			ASSERT(turrets[i]["moveDirection"].Size() == 2);
			v2MoveDirection = vmake(turrets[i]["moveDirection"][0].GetFloat(), turrets[i]["moveDirection"][1].GetFloat());
		}
			
		bool bShuffleAim = true;
		if (turrets[i].HasMember("shuffleAim")) {
			bShuffleAim = turrets[i]["shuffleAim"].GetBool();
		}

		ASSERT(turrets[i].HasMember("aimDirections"));
		const Value& directions = turrets[i]["aimDirections"];
		for (SizeType j = 0; j < directions.Size(); j++) {
			ASSERT(directions[j].Size() == 2);
			vAimDirections.push_back(vmake(directions[j][0].GetFloat(), directions[j][1].GetFloat()));
		}

		// Create turrets
		Entity* pTurret = g_pEntitiesFactory->createEnemy(v2Pos, Entity::EENEMYTURRET, v2MoveDirection, vAimDirections, bShuffleAim);
		addEntity(pTurret);
		++m_iCurrentEnemies;
	}
	fclose(pFile);

	// Create player and first pickup
	Entity* pPlayer = g_pEntitiesFactory->createPlayer(vmake(WORLD_WIDTH * 0.5f, WORLD_HEIGHT * 0.5f));
	addEntity(pPlayer);

	Entity* pPickup = g_pEntitiesFactory->createWeaponPickup();
	addEntity(pPickup);

	// Generate spawn points
	m_vSpawnPositions.push_back(vmake(WORLD_WIDTH / 2.0f, WORLD_HEIGHT));
	m_vSpawnPositions.push_back(vmake(WORLD_WIDTH, WORLD_HEIGHT / 2.0f));
	m_vSpawnPositions.push_back(vmake(0.0f, WORLD_HEIGHT / 2.0f));
	m_vSpawnPositions.push_back(vmake(WORLD_WIDTH / 2.0f, 0.0f));

	return true;
}

void CWorld::cleanup() {
	for (size_t i = 0; i < m_vEntities.size(); ++i) {
		DELETE(m_vEntities[i]);
	}
	m_vEntities.clear();

	for (size_t i = 0; i < m_vEntitiesToAdd.size(); ++i) {
		DELETE(m_vEntitiesToAdd[i]);
	}
	m_vEntitiesToAdd.clear();

	m_vEntitiesToRemove.clear();

	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::TEventType::EPause);
	}
}

void CWorld::run(float _fDeltaTime) {
	if (!m_bIsPaused && !m_bIsGameOver) {

		for (size_t i = 0; i < m_vEntities.size(); ++i) {
			m_vEntities[i]->run(_fDeltaTime);
		}
		checkCollisions();
		removePendingEntities();

		if (m_bIsGameOver) {
			g_pWorld->cleanup();
			std::string scoreMessage = g_pStringManager->getText("LTEXT_GUI_SCORE_MESSAGE") + std::to_string(m_uScore);
			g_pMenuManager->getMenu(MenuManager::EGameOverMenu)->setTitle(scoreMessage.c_str());
			g_pMenuManager->activateMenu(MenuManager::EGameOverMenu);
		}
		else {
			spawnNewEntities();
			addPendingEntities();
		}
	}
}

void CWorld::addEntity(Entity* _pEntity) {
	ASSERT(_pEntity);
	m_vEntitiesToAdd.push_back(_pEntity);
}

void CWorld::removeEntity(Entity* _pEntity) {
	ASSERT(_pEntity);
	m_vEntitiesToRemove.push_back(_pEntity);
}

bool CWorld::onEvent(const IInputManager::Event& _event) {
	IInputManager::TEventType eEventType = _event.getType();
	if (eEventType == IInputManager::TEventType::EPause) {
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
		Entity* pEntity1 = m_vEntities[i];
		for (size_t j = i + 1; j < m_vEntities.size(); ++j) {
			Entity* pEntity2 = m_vEntities[j];
			MessageCheckCollision msgCheckCollision;
			msgCheckCollision.other = pEntity2;
			pEntity1->receiveMessage(&msgCheckCollision);
		}
	}
}

void CWorld::removePendingEntities() {
	for (auto it = m_vEntitiesToRemove.begin(); it != m_vEntitiesToRemove.end(); ++it) {
		Entity::TType type = (*it)->getType();
		switch (type) {
			case Entity::EPLAYER: {
				m_bIsGameOver = true;
				break;
			}
			case Entity::EPICKUP: {
				m_iPickupSpawnTimer = 0;
				break;
			}

#define REG_ENTITY(val, name) \
			case Entity::E##val: \
				if (m_iCurrentEnemies == m_iMaxEnemies) { \
					m_iEnemySpawnTimer = 0; \
				} \
				--m_iCurrentEnemies; \
				break;
#include "REG_ENEMIES.h"
#undef REG_ENTITY

			case Entity::EHUDMESSAGE: {
				if (m_pHudMessage == *it) {
					m_pHudMessage = nullptr;
				}
				break;
			}
		}

		m_vEntities.erase(
			std::remove(m_vEntities.begin(), m_vEntities.end(), *it),
			m_vEntities.end()
		);
		DELETE(*it);
	}
	m_vEntitiesToRemove.clear();
}

void CWorld::addPendingEntities() {
	for (size_t i = 0; i < m_vEntitiesToAdd.size(); ++i) {
		m_vEntities.push_back(m_vEntitiesToAdd[i]);
		switch (m_vEntitiesToAdd[i]->getType()){
			case Entity::EPLAYER: 
				m_pPlayer = m_vEntitiesToAdd[i];
				break;
			case Entity::EHUDMESSAGE:
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
			Entity* pPickup = g_pEntitiesFactory->createWeaponPickup();
			addEntity(pPickup);
		}
	}

	// Spawn Enemy
	++m_iEnemySpawnTimer;

	if (m_iEnemySpawnTimer >= m_iEnemySpawnWait && m_iCurrentEnemies < m_iMaxEnemies) {
		// Pick a random spawn point from the available ones
		vec2 spawnLocation = m_vSpawnPositions[rand() % m_vSpawnPositions.size()];

		// Pick a random enemy based on the configure probability of each one
		float enemyType = CORE_FRand(0.0f, 1.0f);
		for (auto itEnemyProbability = m_mEnemyProbabilities.begin(); itEnemyProbability != m_mEnemyProbabilities.end(); ++itEnemyProbability) {
			if (enemyType <= itEnemyProbability->second) {
				addEntity(g_pEntitiesFactory->createEnemy(spawnLocation, itEnemyProbability->first, m_pPlayer));
				break;
			}
		}
		
		++m_iCurrentEnemies;
		m_iEnemySpawnTimer = 0;
	}
}
