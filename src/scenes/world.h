#pragma once

#include "../input/input_manager.h"

class Entity;

class World : public IInputManager::IListener {
public:
	enum TEnemyType {
		EMelee,
		EBig,
		ERange,
	};

	struct TEnemyData {
		TEnemyType type;
		int        speed;
		int        life;
		int        damage;
		float      spawnProbability;
	};

	World(uint16_t level) : m_level(level) {}
	~World();

	void         init         ();
	void         destroy      ();
	void         run          (float deltaTime);
	void         addEntity    (Entity* entity);
	void         removeEntity (Entity* entity);
	uint16_t     getScore     () const { return m_score; }
	Entity*      getPlayer    () const { return m_player; }
	Entity*      getHUDMessage() const { return m_hudMessage; }
	void         addPoints    (uint16_t points)    { m_score += points; }
	virtual bool onEvent      (const IInputManager::Event& event);
private:
	bool loadLevel            (const char* fileName);
	void checkCollisions      ();
	void removePendingEntities();
	void addPendingEntities   ();
	void spawnEnemy           ();

	uint16_t              m_level;
	Entity*               m_player;
	Entity*               m_hudMessage;
	std::vector <Entity*> m_entities;
	std::vector <Entity*> m_entitiesToRemove;
	std::vector <Entity*> m_entitiesToAdd;
	bool                  m_isGameOver;
	bool                  m_isPaused;

	// Game rules
	uint16_t m_score;
	int      m_pickupSpawnWait;
	int      m_enemySpawnWait;
	int      m_spawnPoints;
	int      m_currentEnemies;
	int      m_maxEnemies;

	std::vector<TEnemyData> m_enemyData;
	std::vector<vec2>       m_spawnData;

	// Timers
	int m_pickupSpawnTimer;
	int m_enemySpawnTimer;
};

// Collision utils
inline float clamp(float value, float inf, float sup) { return (value <= inf) ? inf : (value >= sup) ? sup : value; };
inline bool  isInBounds(float value, float inf, float sup) { return (value > inf) && (value < sup); };
bool checkCircleCircle(const vec2& pos1, float radius1, const vec2& pos2, float radius2);
bool checkCircleRect(const vec2& circlePos, float circleRadius, const vec2& rectPos, const vec2& rectSize);
bool checkRectRect(const vec2& rectPos1, const vec2& rectSize1, const vec2& rectPos2, const vec2& rectSize2);