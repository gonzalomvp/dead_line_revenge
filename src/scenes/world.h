#pragma once

#include "../input/input_manager.h"
#include "../entities/components/component.h"
#include "../entities/entity.h"

#define WORLD_WIDTH  640
#define WORLD_HEIGHT 480


class World : public IInputManager::IListener {
public:
	enum TEnemyType {
		EMelee,
		EBig,
		ERange,
		ETurret,
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

	// Data
	Component::TWeaponData getWeaponData(Component::TWeapon weaponType);
	
	// Entity creation methods
	Entity* createPlayer    (vec2 pos);
	Entity* createBullet    (vec2 pos, vec2 size, vec2 direction, float speed, int damage, int life, int range, bool isExplossive, bool isBouncy, Entity::TType entityType, const char* texture);
	Entity* createExplossion(vec2 pos, vec2 size, vec2 sizeIncrement, int duration, Entity::TType entityType);


	Entity* createEnemy(int x, int y, Entity* player, int speed, int lives, int damage);
	Entity* createBigEnemy(int x, int y, Entity* player, int speed, int lives, int damage);
	Entity* createRangeEnemy(int x, int y, Entity* player);
	Entity* createTurretEnemy(vec2 position, vec2 moveDir, std::vector<vec2> aimDirections, bool shuffleAim);

	Entity* createWeaponPickup();
	
	Entity* createHUDMessage(std::string, vec2 pos, int displayTime);

	//quitar de aqui
	
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
	int      m_playerSpeed = 5; //Leer de fichero
	int      m_pickupSpawnWait;
	int      m_enemySpawnWait;
	int      m_currentEnemies;
	int      m_maxEnemies;

	std::map<Component::TWeapon, Component::TWeaponData> m_weaponData;
	
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