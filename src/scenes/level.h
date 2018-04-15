#pragma once

#include "../common/stdafx.h"
#include "../common/sys.h"
#include "../common/core.h"

class Entity;

enum TEnemyType {
	EMelee,
	EBig,
	ERange,
};

struct TEnemyData {
	TEnemyType type;
	int speed;
	int life;
	int damage;
	float spawnProbability;
};

class Level {
public:
	static Level* loadLevel(char* fileName);
	Level();
	void run();
	void killEnemy(Entity* entity);
	void addPoints(int points) { m_score += points; }
	int m_score;

private:
	void spawnEnemy();
	std::vector<vec2> m_spawnLocations;
	int m_spawnPoints;
	float m_spawnFrecuency;
	float m_spawnTimer;
	int m_maxEnemies;
	int m_numEnemies;
	std::vector<TEnemyData> m_enemyData;
	std::vector<Entity*> m_enemies;
	
};