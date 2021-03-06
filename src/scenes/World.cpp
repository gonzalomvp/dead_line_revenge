#include "common/stdafx.h"
#include "World.h"

#include "entities/EntitiesFactory.h"
#include "messages/Message.h"
#include "gui/MenuManager.h"
#include "gui/StringManager.h"
#include "gui/controls/Label.h"
#include "gui/controls/Menu.h"

#include <algorithm>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

namespace {
	const std::string s_sLevel1File = "data/config/level1.json";
	const std::string s_sLevel2File = "data/config/level2.json";
	const std::string s_sLevel3File = "data/config/level3.json";
}

CWorld::CWorld() 
: m_uLevel(0)
, m_pPlayer(nullptr)
, m_bIsGameOver(false)
, m_bIsPaused(false)
, m_uScore(0)
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
	m_bIsGameOver = false;
	m_bIsPaused = false;
	m_uScore = 0;
	m_fPickupSpawnWait = 0.0f;
	m_fEnemySpawnWait = 0.0f;
	m_iCurrentEnemies = 0;
	m_iMaxEnemies = 0;
	m_iBossSpawnPoints = 0;
	m_iBossSpawnPointsCounter = 0;
	m_fPickupSpawnTimer = 0.0f;
	m_fEnemySpawnTimer = 0.0f;

	m_vEntities.clear();
	m_vEntitiesToRemove.clear();
	m_vEntitiesToAdd.clear();

	m_mEnemyProbabilities.clear();
	m_vSpawnPositions.clear();

	ASSERT(g_pInputManager);
	g_pInputManager->registerEvent(this, IInputManager::EEventType::EPause);

	// Parse level file
	const char* psFileName = "";
	switch (_uLevel) {
		case 1:
			psFileName = s_sLevel1File.c_str();
			break;
		case 2:
			psFileName = s_sLevel2File.c_str();
			break;
		case 3:
			psFileName = s_sLevel3File.c_str();
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

	// Load player level configuration
	ASSERT(doc.HasMember("player"));
	const Value& player = doc["player"];
	
	ASSERT(player.HasMember("life"));
	int iPlayerLife = player["life"].GetInt();
	
	ASSERT(player.HasMember("startPosition") && player["startPosition"].Size() == 2);
	vec2 v2StartPos = vmake(player["startPosition"][0].GetFloat(), player["startPosition"][1].GetFloat());

	// Create player
	m_pPlayer = g_pEntitiesFactory->createPlayer(v2StartPos, iPlayerLife);
	addEntity(m_pPlayer);

	// Load pickup level configuration
	ASSERT(doc.HasMember("pickup"));
	const Value& pickup = doc["pickup"];
	
	ASSERT(pickup.HasMember("spawnWait"));
	m_fPickupSpawnWait = pickup["spawnWait"].GetFloat();

	// Create first pickup
	CEntity* pPickup = g_pEntitiesFactory->createWeaponPickup();
	addEntity(pPickup);

	// Load enemy level configuration
	float fTotalProbability = 0.0f;
	ASSERT(doc.HasMember("enemies"));
	const Value& enemies = doc["enemies"];

	ASSERT(enemies.HasMember("spawnWait"));
	m_fEnemySpawnWait = enemies["spawnWait"].GetFloat();
	m_fEnemySpawnTimer = m_fEnemySpawnWait;
	
	ASSERT(enemies.HasMember("maxEnemies"));
	m_iMaxEnemies = enemies["maxEnemies"].GetInt();

	ASSERT(enemies.HasMember("bossSpawnPoints"));
	m_iBossSpawnPoints = enemies["bossSpawnPoints"].GetInt();
	m_iBossSpawnPointsCounter = m_iBossSpawnPoints;

	ASSERT(enemies.HasMember("spawnProbabilities"));
	const Value& spawnProbabilities = enemies["spawnProbabilities"];

	for (SizeType i = 0; i < spawnProbabilities.Size(); ++i) {
		ASSERT(spawnProbabilities[i].HasMember("type"));
		CEntity::EType eEnemyType = CEntity::getEntityTypeByName(spawnProbabilities[i]["type"].GetString());
		
		ASSERT(spawnProbabilities[i].HasMember("spawnProbability"));
		fTotalProbability += spawnProbabilities[i]["spawnProbability"].GetFloat();
		m_mEnemyProbabilities[eEnemyType] = fTotalProbability;
	}
	ASSERT(fTotalProbability == 1.f);

	// Load turret level configuration
	if (enemies.HasMember("turrets")) {
		const Value& turrets = enemies["turrets"];
		for (SizeType i = 0; i < turrets.Size(); ++i) {
			ASSERT(turrets[i].HasMember("position") && turrets[i]["position"].Size() == 2);
			vec2 v2Pos = vmake(turrets[i]["position"][0].GetFloat(), turrets[i]["position"][1].GetFloat());

			vec2 v2MoveDirection = vmake(0.0f, 0.0f);
			if (turrets[i].HasMember("moveDirection")) {
				ASSERT(turrets[i]["moveDirection"].Size() == 2);
				v2MoveDirection = vmake(turrets[i]["moveDirection"][0].GetFloat(), turrets[i]["moveDirection"][1].GetFloat());
			}

			vec2 v2AimDirection = vmake(1.0f, 0.0f);
			if (turrets[i].HasMember("aimDirection")) {
				ASSERT(turrets[i]["aimDirection"].Size() == 2);
				v2AimDirection = vmake(turrets[i]["aimDirection"][0].GetFloat(), turrets[i]["aimDirection"][1].GetFloat());
			}

			std::string sBTFile = "";
			if (turrets[i].HasMember("behaviorTree")) {
				sBTFile = turrets[i]["behaviorTree"].GetString();
			}

			// Create turret
			CEntity* pTurret = g_pEntitiesFactory->createEnemy(v2Pos, CEntity::EENEMYTURRET, sBTFile, v2MoveDirection, v2AimDirection);
			addEntity(pTurret);
			++m_iCurrentEnemies;
		}
	}
	
	fclose(pFile);

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
			std::string sScoreTMessage = g_pStringManager->getText(menu::constants::s_psScoreText) + std::to_string(m_uScore);
			CControl* pControl = pGameOverMenu->getControlByName(menu::constants::s_psScoreLabelName);
			if (pControl) {
				CLabel* pLabel = dynamic_cast<CLabel*>(pControl);
				if (pLabel) {
					pLabel->setText(sScoreTMessage);
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

void CWorld::addEntity(CEntity* _pEntity) {
	ASSERT(_pEntity);
	m_vEntitiesToAdd.push_back(_pEntity);
}

void CWorld::removeEntity(CEntity* _pEntity) {
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
		CEntity* pEntity1 = m_vEntities[i];
		for (size_t j = i + 1; j < m_vEntities.size(); ++j) {
			CEntity* pEntity2 = m_vEntities[j];
			TMessageCheckCollision msgCheckCollision;
			msgCheckCollision.pOther = pEntity2;
			pEntity1->receiveMessage(&msgCheckCollision);
		}
	}
}

void CWorld::removePendingEntities() {
	for (auto it = m_vEntitiesToRemove.begin(); it != m_vEntitiesToRemove.end(); ++it) {
		CEntity::EType eEntityType = (*it)->getType();
		switch (eEntityType) {
			case CEntity::EPLAYER: {
				m_bIsGameOver = true;
				break;
			}
			case CEntity::EPICKUP: {
				m_fPickupSpawnTimer = m_fPickupSpawnWait;
				break;
			}

#define REG_ENTITY(val, name) \
			case CEntity::E##val: \
				--m_iCurrentEnemies; \
				break;
#include "REG_ENEMIES.h"
#undef REG_ENTITY

		}

		// Enable enemies spawn after killing the Boss
		if (eEntityType == CEntity::EENEMYBOSS) {
			m_fEnemySpawnTimer = m_fEnemySpawnWait;
			m_iBossSpawnPointsCounter = m_uScore + m_iBossSpawnPoints;
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
	}
	m_vEntitiesToAdd.clear();
}

void CWorld::spawnNewEntities(float _fDeltaTime) {
	// Spawn Pickup
	if (m_fPickupSpawnTimer > 0.0f) {
		m_fPickupSpawnTimer -= _fDeltaTime;
		if (m_fPickupSpawnTimer <= 0.0f) {
			CEntity* pPickup = g_pEntitiesFactory->createWeaponPickup();
			addEntity(pPickup);
		}
	}

	// Spawn Enemy
	if (m_fEnemySpawnTimer > 0.0f && m_iCurrentEnemies < m_iMaxEnemies) {
		m_fEnemySpawnTimer -= _fDeltaTime;
		if (m_fEnemySpawnTimer <= 0.0f) {
			// Pick a random spawn point from the available ones
			vec2 v2SpawnLocation = m_vSpawnPositions[rand() % m_vSpawnPositions.size()];
			CEntity* enemy = nullptr;

			// If enough points spawn Boss
			if (m_uScore >= m_iBossSpawnPointsCounter) {
				enemy = g_pEntitiesFactory->createEnemy(v2SpawnLocation, CEntity::EENEMYBOSS);
			}
			// Otherwise pick a random enemy based on the probability for each one
			else {
				float fEnemyTypeProb = CORE_FRand(0.0f, 1.0f);
				for (auto itEnemyProbability = m_mEnemyProbabilities.begin(); itEnemyProbability != m_mEnemyProbabilities.end(); ++itEnemyProbability) {
					if (fEnemyTypeProb <= itEnemyProbability->second) {
						enemy = g_pEntitiesFactory->createEnemy(v2SpawnLocation, itEnemyProbability->first);
						m_fEnemySpawnTimer = m_fEnemySpawnWait;
						break;
					}
				}
			}
			ASSERT(enemy);
			addEntity(enemy);
			++m_iCurrentEnemies;
		}
	}
}
