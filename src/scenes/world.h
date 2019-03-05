#pragma once

#include "entities/entity.h"
#include "input/input_manager.h"

#define WORLD_WIDTH  640
#define WORLD_HEIGHT 480

class CWorld : public IInputManager::IListener {
public:
	CWorld(uint16_t _uLevel);
	~CWorld();

	void         init           ();
	bool         loadLevel      ();
	void         unloadLevel    ();
	void         run            (float _fDeltaTime);
	void         addEntity      (Entity* _pEntity);
	void         removeEntity   (Entity* _pEntity);
	uint16_t     getScore       () const                             { return m_uScore;        }
	Entity*      getPlayer      () const                             { return m_pPlayer;       }
	int	         getPlayerLife  () const                             { return m_iPlayerLife;   }
	int	         getPlayerSpeed () const                             { return m_fPlayerSpeed;  }


	void         addPoints    (Entity::TType _tEntityType)           { m_uScore += m_mEntityPoints[_tEntityType]; }
	virtual bool onEvent      (const IInputManager::Event& _event);

private:
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
	int      m_iPickupSpawnWait;
	int      m_iEnemySpawnWait;
	int      m_iCurrentEnemies;
	int      m_iMaxEnemies;
	std::map<Entity::TType, float> m_mEnemyProbabilities;
	std::map<Entity::TType, float> m_mEntityPoints;
	std::vector<vec2> m_vSpawnPositions;

	// Timers
	int m_iPickupSpawnTimer;
	int m_iEnemySpawnTimer;
};