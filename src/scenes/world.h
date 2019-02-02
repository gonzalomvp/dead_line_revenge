#pragma once

#include "../input/input_manager.h"
#include"../entities/components/component.h"
#include "../entities/entity.h"

#define WORLD_WIDTH  640
#define WORLD_HEIGHT 480

class World : public IInputManager::IListener {
public:
	struct TEnemyData {
		Entity::TType  type;
		int            life;
		float          speed;
		int            collisionDamage;
		int            fireRate;
		float          bulletSpeed;
		int            bulletDamage;
		int            bulletLife;
		int            bulletRange;
		bool           isExplossive;
		bool           isBouncy;
		float          spawnProbability;
		int            points;
		vec2           size;
		std::string    imageFile;
	};

	World(uint16_t level) : m_level(level) {}
	~World();

	void         init         ();
	bool         loadLevel    ();
	void         unloadLevel  ();
	void         run          (float deltaTime);
	void         addEntity    (ptr<Entity> entity);
	void         removeEntity (ptr<Entity> entity);
	uint16_t     getScore     () const                             { return m_score;    }
	ptr<Entity>      getPlayer    () const                             { return m_player;   }
	void         addPoints    (uint16_t points)                    { m_score += points; }
	virtual bool onEvent      (const IInputManager::Event& event);

	// Entity creation methods
	ptr<Entity> createPlayer      (vec2 pos);
	ptr<Entity> createBullet      (vec2 pos, vec2 size, vec2 direction, float speed, int damage, int life, int range, bool isExplossive, bool isBouncy, Entity::TType entityType, const char* texture);
	ptr<Entity> createExplossion  (vec2 pos, vec2 size, vec2 sizeIncrement, int duration, Entity::TType entityType);
	ptr<Entity> createEnemy       (vec2 pos, TEnemyData enemyData, ptr<Entity> player);
	ptr<Entity> createEnemy       (vec2 pos, TEnemyData enemyData, vec2 moveDir, std::vector<vec2> aimDirections, bool shuffleAim);
	ptr<Entity> createWeaponPickup();
	ptr<Entity> createHUDMessage  (const std::string&, vec2 pos, int displayTime);
private:
	bool loadConfig           ();
	void checkCollisions      ();
	void removePendingEntities();
	void addPendingEntities   ();
	void spawnNewEntities     ();

	uint16_t              m_level;
	ptr<Entity>               m_player;
	ptr<Entity>               m_hudMessage;
	std::vector <ptr<Entity>> m_entities;
	std::vector <ptr<Entity>> m_entitiesToRemove;
	std::vector <ptr<Entity>> m_entitiesToAdd;
	bool                  m_isGameOver;
	bool                  m_isPaused;

	// Game rules
	uint16_t m_score;
	int      m_playerLife;
	float    m_playerSpeed;
	int      m_pickupPoints;
	int      m_pickupSpawnWait;
	int      m_enemySpawnWait;
	int      m_currentEnemies;
	int      m_maxEnemies;

	// Config loaded from file
	std::map<ComponentWeapon::TWeapon, ComponentWeapon::TWeaponData> m_weaponData;
	std::map<Entity::TType, TEnemyData>                              m_enemyData;
	std::vector<vec2>                                                m_spawnData;

	// Timers
	int m_pickupSpawnTimer;
	int m_enemySpawnTimer;
};