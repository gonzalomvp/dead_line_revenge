#include "common/stdafx.h"
#include "scenes/world.h"

#include "entities/entities_factory.h"
#include "messages/message.h"
#include "gui/menu_manager.h"
#include "gui/string_manager.h"
#include "gui/controls/label.h"
#include "gui/controls/menu.h"

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
, m_fPickupSpawnWait(0.0f)
, m_fEnemySpawnWait(0.0f)
, m_iCurrentEnemies(0)
, m_iMaxEnemies(0)
, m_fPickupSpawnTimer(0.0f)
, m_fEnemySpawnTimer(0.0f)
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
	m_fPickupSpawnWait = 0.0f;
	m_fEnemySpawnWait = 0.0f;
	m_iCurrentEnemies = 0;
	m_iMaxEnemies = 0;
	m_fPickupSpawnTimer = 0.0f;
	m_fEnemySpawnTimer = 0.0f;

	m_vEntities.clear();
	m_vEntitiesToRemove.clear();
	m_vEntitiesToAdd.clear();

	m_mEnemyProbabilities.clear();
	m_mEntityPoints.clear();
	m_vSpawnPositions.clear();

	ASSERT(g_pInputManager);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EPause);

	// Parse level file
	const char* psFileName = "";
	switch (_uLevel) {
		case 1:
			psFileName = "data/level1.json";

			// Force test level
			psFileName = "data/levelTest.json";
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
	m_fPickupSpawnWait = doc["pickupSpawnWait"].GetFloat();
	ASSERT(doc.HasMember("enemySpawnWait"));
	m_fEnemySpawnWait = doc["enemySpawnWait"].GetFloat();
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
		Entity::EType eEnemyType = CEntitiesFactory::getEntityTypeByName(enemies[i]["type"].GetString());
		ASSERT(enemies[i].HasMember("spawnProbability"));
		fTotalProbability += enemies[i]["spawnProbability"].GetFloat();
		m_mEnemyProbabilities[eEnemyType] = fTotalProbability;
		ASSERT(enemies[i].HasMember("points"));
		m_mEntityPoints[eEnemyType] = enemies[i]["points"].GetUint();
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
		const Value& aimDirections = turrets[i]["aimDirections"];
		for (SizeType j = 0; j < aimDirections.Size(); j++) {
			ASSERT(aimDirections[j].Size() == 2);
			vAimDirections.push_back(vmake(aimDirections[j][0].GetFloat(), aimDirections[j][1].GetFloat()));
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
	m_pPlayer = pPlayer;

	Entity* pPickup = g_pEntitiesFactory->createWeaponPickup();
	addEntity(pPickup);
	m_fPickupSpawnTimer = m_fPickupSpawnWait;

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
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EPause);
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
			cleanup();

			// Show Game Over Menu with the score
			CMenu* pGameOverMenu = g_pMenuManager->getMenu(CMenuManager::EGameOverMenu);
			ASSERT(pGameOverMenu);
			std::string sScoreMessage = g_pStringManager->getText(menu::constants::s_psScoreText) + std::to_string(m_uScore);
			CControl* pControl = pGameOverMenu->getControlByName(menu::constants::s_psScoreLabelName);
			if (pControl) {
				CLabel* pLabel = dynamic_cast<CLabel*>(pControl);
				if (pLabel) {
					pLabel->setText(sScoreMessage);
				}
			}
			g_pMenuManager->activateMenu(CMenuManager::EGameOverMenu);
		}
		else {
			spawnNewEntities(_fDeltaTime);
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

bool CWorld::onEvent(const IInputManager::CEvent& _event) {
	bool bConsumed = false;
	IInputManager::EEventType eEventType = _event.getType();
	if (eEventType == IInputManager::EEventType::EPause) {
		bConsumed = true;
		m_bIsPaused = !m_bIsPaused;
		if (m_bIsPaused) {
			m_pPlayer->deactivate();
			g_pMenuManager->activateMenu(CMenuManager::EPauseMenu);
		}
		else {
			m_pPlayer->activate();
			g_pMenuManager->deactivateMenu();
		}
	}
	return bConsumed;
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
		Entity::EType eEntityType = (*it)->getType();
		switch (eEntityType) {
			case Entity::EPLAYER: {
				m_bIsGameOver = true;
				break;
			}
			case Entity::EPICKUP: {
				m_fPickupSpawnTimer = 0.0f;
				break;
			}

#define REG_ENTITY(val, name) \
			case Entity::E##val: \
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

void CWorld::spawnNewEntities(float _fDeltaTime) {
	// Spawn Pickup
	if (m_fPickupSpawnTimer < m_fPickupSpawnWait) {
		m_fPickupSpawnTimer += _fDeltaTime;
		if (m_fPickupSpawnTimer >= m_fPickupSpawnWait) {
			Entity* pPickup = g_pEntitiesFactory->createWeaponPickup();
			addEntity(pPickup);
		}
	}
	
	// Spawn Enemy
	if (m_fEnemySpawnTimer < m_fEnemySpawnWait && m_iCurrentEnemies < m_iMaxEnemies) {
		m_fEnemySpawnTimer += _fDeltaTime;
		if (m_fEnemySpawnTimer >= m_fEnemySpawnWait) {
			// Pick a random spawn point from the available ones
			vec2 v2SpawnLocation = m_vSpawnPositions[rand() % m_vSpawnPositions.size()];

			// Pick a random enemy based on the probability for each one
			float fEnemyTypeProb = CORE_FRand(0.0f, 1.0f);
			for (auto itEnemyProbability = m_mEnemyProbabilities.begin(); itEnemyProbability != m_mEnemyProbabilities.end(); ++itEnemyProbability) {
				if (fEnemyTypeProb <= itEnemyProbability->second) {
					Entity* enemy = g_pEntitiesFactory->createEnemy(v2SpawnLocation, itEnemyProbability->first, m_pPlayer);
					addEntity(enemy);
					++m_iCurrentEnemies;
					m_fEnemySpawnTimer = 0.0f;
					break;
				}
			}
		}
	}
}
