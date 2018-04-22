#include "../common/stdafx.h"
#include "level.h"
#include "../globals.h"
#include "world.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include <fstream>

using namespace rapidjson;

Level* Level::loadLevel(char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (!file) return nullptr;

	fseek(file, 0, SEEK_END);
	std::vector<char> fileData(ftell(file));
	fseek(file, 0, SEEK_SET);
	FileReadStream is(file, fileData.data(), fileData.size());
	Document doc;
	doc.ParseStream(is);

	float totalProbability = 0.0f;
	int spawnPoints = doc["spawnPoints"].GetInt();
	int spawnFrecuency = doc["spawnFrecuency"].GetInt();
	int concurrentEnemies = doc["concurrentEnemies"].GetInt();

	TEnemyData meleeEnemy;
	meleeEnemy.type = EMelee;
	meleeEnemy.speed = doc["meleeEnemy"].GetObject()["speed"].GetInt();
	meleeEnemy.life = doc["meleeEnemy"].GetObject()["life"].GetInt();
	meleeEnemy.damage = doc["meleeEnemy"].GetObject()["damage"].GetInt();
	totalProbability += doc["meleeEnemy"].GetObject()["spawnProbability"].GetFloat();
	meleeEnemy.spawnProbability = totalProbability;

	TEnemyData bigEnemy;
	bigEnemy.type = EBig;
	bigEnemy.speed = doc["bigEnemy"].GetObject()["speed"].GetInt();
	bigEnemy.life = doc["bigEnemy"].GetObject()["life"].GetInt();
	bigEnemy.damage = doc["bigEnemy"].GetObject()["damage"].GetInt();
	totalProbability += doc["bigEnemy"].GetObject()["spawnProbability"].GetFloat();
	bigEnemy.spawnProbability = totalProbability;

	TEnemyData rangeEnemy;
	rangeEnemy.type = ERange;
	rangeEnemy.speed = doc["rangeEnemy"].GetObject()["speed"].GetInt();
	rangeEnemy.life = doc["rangeEnemy"].GetObject()["life"].GetInt();
	rangeEnemy.damage = doc["rangeEnemy"].GetObject()["damage"].GetInt();
	totalProbability += doc["rangeEnemy"].GetObject()["spawnProbability"].GetFloat();
	rangeEnemy.spawnProbability = totalProbability;

	Level* level = new Level();
	level->m_spawnPoints = spawnPoints;
	level->m_spawnFrecuency = spawnFrecuency;
	level->m_maxEnemies = concurrentEnemies;

	level->m_enemyData.push_back(meleeEnemy);
	level->m_enemyData.push_back(bigEnemy);
	level->m_enemyData.push_back(rangeEnemy);

	fclose(file);
	return level;
}

Level::Level() {
	m_spawnLocations.push_back(vmake(SCR_WIDTH / 2.0f, SCR_HEIGHT));
	m_spawnLocations.push_back(vmake(SCR_WIDTH, SCR_HEIGHT / 2.0f));
	m_spawnLocations.push_back(vmake(0.0f, SCR_HEIGHT / 2.0f));
	m_spawnLocations.push_back(vmake(SCR_WIDTH / 2.0f, 0.0f));
	//m_maxEnemies = 3;
	//m_spawnFrecuency = 60;
	m_numEnemies = 0;
	m_spawnTimer = 60;
	m_score = 0;

	//g_world->addEntity(createTurretEnemy(100, SCR_HEIGHT - 100, vmake(0, -1)));
}

void Level::run() {
	++m_spawnTimer;
	
	if (m_spawnTimer >= m_spawnFrecuency && m_numEnemies < m_maxEnemies) {
		spawnEnemy();
		m_spawnTimer = 0;
	}
}

void Level::spawnEnemy() {
	vec2 spawnLocation = m_spawnLocations[rand() % m_spawnPoints];
	float enemyType = CORE_FRand(0.0f, 1.0f);
	Entity* enemy = nullptr;
	for (size_t i = 0; i < m_enemyData.size(); i++)
	{
		if (enemyType <= m_enemyData[i].spawnProbability) {
			switch (m_enemyData[i].type) {
			case EMelee:
				enemy = Entity::createEnemy(spawnLocation.x, spawnLocation.y, g_world->getPlayer(), m_enemyData[i].speed, m_enemyData[i].life, m_enemyData[i].damage);
				break;
			case EBig:
				enemy = Entity::createBigEnemy(spawnLocation.x, spawnLocation.y, g_world->getPlayer(), m_enemyData[i].speed, m_enemyData[i].life, m_enemyData[i].damage);
				break;
			case ERange:
				enemy = Entity::createRangeEnemy(spawnLocation.x, spawnLocation.y, g_world->getPlayer());
				break;
			}
			break;
		}
	}

	g_world->addEntity(enemy);
	m_enemies.push_back(enemy);
	++m_numEnemies;
}

void Level::killEnemy(Entity* entity) {
	for (auto itEnemy = m_enemies.begin(); itEnemy != m_enemies.end(); ++itEnemy) {
		if (*itEnemy == entity) {
			m_enemies.erase(itEnemy);
			--m_numEnemies;
			break;
		}
	}
}