#pragma once

#include "../input/input_manager.h"
#include "../entities/components/component.h"

#define WORLD_WIDTH  640
#define WORLD_HEIGHT 420

class Entity;

class World : public IInputManager::IListener {
public:
	enum TEnemyType {
		EMelee,
		EBig,
		ERange,
		ETurret,
	};

	struct TWeaponData {
		Component::TWeapon type;
		int                fireRate;
		int                reloadTime;
		int                capacity;
		int                bulletSpeed;
		int                bulletDamage;
		int                bulletLife;
		int                bulletRange;
		bool               isAutomatic;
		bool               isExplossive;
		bool               isBouncy;
		char               soundFilename[50];
	};

	struct TEnemyData {
		TEnemyType type;
		int        life;
		int        speed;
		int        collisionDamage;
		int        fireRate;
		int        bulletSpeed;
		int        bulletDamage;
		int        bulletLife;
		int        bulletRange;
		bool       isExplossive;
		bool       isBouncy;
		float      spawnProbability;
		int        points;
	};

	World(uint16_t level) : m_level(level) { loadConfig(); }
	~World();

	void         init         ();
	void         destroy      ();
	void         run          (float deltaTime);
	void         addEntity    (Entity* entity);
	void         removeEntity (Entity* entity);
	uint16_t     getScore     () const          { return m_score; }
	Entity*      getPlayer    () const          { return m_player; }
	Entity*      getHUDMessage() const          { return m_hudMessage; }
	void         addPoints    (uint16_t points) { m_score += points; }
	virtual bool onEvent      (const IInputManager::Event& event);

	//quitar de aqui
	std::map<Component::TWeapon, TWeaponData> m_weaponData;
	std::map<TEnemyType, TEnemyData>  m_enemyData;
private:
	bool loadLevel            (const char* fileName);
	bool loadConfig           ();
	void checkCollisions      ();
	void removePendingEntities();
	void addPendingEntities   ();
	void spawnNewEntities     ();
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
	int      m_playerLife;
	int      m_pickupSpawnWait;
	int      m_enemySpawnWait;
	int      m_currentEnemies;
	int      m_maxEnemies;

	
	
	std::vector<vec2>        m_spawnData;

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