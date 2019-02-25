#pragma once

#include "entities/components/component.h"
#include "entities/entity.h"
#include "input/input_manager.h"

#define WORLD_WIDTH  640
#define WORLD_HEIGHT 480

class CWorld : public IInputManager::IListener {
public:
	struct TEnemyData {
		Entity::TType            eType;
		int                      iLife;
		float                    fSpeed;
		int                      iCollisionDamage;
		ComponentWeapon::TType   eWeapon;
		float                    fSpawnProbability;
		int                      iPoints;
		vec2                     v2Size;
		std::string              sImageFile;
	};

	CWorld(uint16_t _uLevel) : m_uLevel(_uLevel) {}
	~CWorld();

	void         init         ();
	bool         loadLevel    ();
	void         unloadLevel  ();
	void         run          (float _fDeltaTime);
	void         addEntity    (Entity* _pEntity);
	void         removeEntity (Entity* _pEntity);
	uint16_t     getScore     () const                             { return m_uScore;      }
	Entity*      getPlayer    () const                             { return m_pPlayer;     }
	void         addPoints    (uint16_t _uPoints)                  { m_uScore += _uPoints; }
	virtual bool onEvent      (const IInputManager::Event& _event);

	// Entity creation methods
	Entity* createPlayer      (vec2 _v2Pos);
	Entity* createBullet      (vec2 _v2Pos, vec2 _v2Size, vec2 _v2Direction, float _fSpeed, int _iDamage, int _iLife, int _iRange, bool _bIsExplossive, bool _bIsBouncy, Entity::TType _eEntityType, const char* _psTexture);
	Entity* createExplossion  (vec2 _v2Pos, vec2 _v2Size, vec2 _v2SizeIncrement, int _iDuration, Entity::TType _eEntityType);
	Entity* createEnemy       (vec2 _v2Pos, TEnemyData _tEnemyData, Entity* _pPlayer);
	Entity* createEnemy       (vec2 _v2Pos, TEnemyData _tEnemyData, vec2 _v2MoveDir, std::vector<vec2> _vAimDirections, bool _bIshuffleAim);
	Entity* createWeaponPickup();
	Entity* createHUDMessage  (const std::string& _sMessage, vec2 _v2Pos, int _iDisplayTime);

	std::map<ComponentWeapon::TType, ComponentWeapon::TWeaponData> m_mWeaponData;

private:
	bool loadConfig           ();
	void checkCollisions      ();
	void removePendingEntities();
	void addPendingEntities   ();
	void spawnNewEntities     ();

	uint16_t              m_uLevel;
	Entity*               m_pPlayer;
	Entity*               m_pHudMessage;
	std::vector <Entity*> m_vEntities;
	std::vector <Entity*> m_vEntitiesToRemove;
	std::vector <Entity*> m_vEntitiesToAdd;
	bool                  m_bIsGameOver;
	bool                  m_bIsPaused;

	// Game rules
	uint16_t m_uScore;
	int      m_iPlayerLife;
	float    m_fPlayerSpeed;
	int      m_iPickupPoints;
	int      m_iPickupSpawnWait;
	int      m_iEnemySpawnWait;
	int      m_iCurrentEnemies;
	int      m_iMaxEnemies;

	// Config loaded from file
	std::map<Entity::TType, TEnemyData> m_mEnemyData;
	std::vector<vec2>                   m_vSpawnData;

	// Timers
	int m_iPickupSpawnTimer;
	int m_iEnemySpawnTimer;
};