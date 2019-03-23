#pragma once

#include "entities/entity.h"
#include "input/input_manager.h"

#define WORLD_WIDTH  640
#define WORLD_HEIGHT 480

class CWorld : public IInputManager::IListener {
public:
	CWorld();
	~CWorld();

	bool         init           (uint16_t _uLevel);
	void         cleanup        ();
	void         run            (float _fDeltaTime);
	void         addEntity      (Entity* _pEntity);
	void         removeEntity   (Entity* _pEntity);
	uint16_t     getLevel       () const                     { return m_uLevel;                           }
	uint16_t     getScore       () const                     { return m_uScore;                           }
	Entity*      getPlayer      () const                     { return m_pPlayer;                          }
	int	         getPlayerLife  () const                     { return m_iPlayerLife;                      }
	float	     getPlayerSpeed () const                     { return m_fPlayerSpeed;                     }
	void         addPoints      (Entity::EType _tEntityType) { m_uScore += m_mEntityPoints[_tEntityType]; }

	// IInputManager::IListener
	virtual bool onEvent (const IInputManager::CEvent& _event);

private:
	void checkCollisions      ();
	void removePendingEntities();
	void addPendingEntities   ();
	void spawnNewEntities     (float _fDeltaTime);

	uint16_t              m_uLevel;
	Entity*               m_pPlayer;
	std::vector <Entity*> m_vEntities;
	std::vector <Entity*> m_vEntitiesToRemove;
	std::vector <Entity*> m_vEntitiesToAdd;
	bool                  m_bIsGameOver;
	bool                  m_bIsPaused;

	// Game rules
	uint16_t m_uScore;
	int      m_iPlayerLife;
	float    m_fPlayerSpeed;
	float    m_fPickupSpawnWait;
	float    m_fEnemySpawnWait;
	int      m_iCurrentEnemies;
	int      m_iMaxEnemies;
	std::map<Entity::EType, float>    m_mEnemyProbabilities;
	std::map<Entity::EType, uint16_t> m_mEntityPoints;
	std::vector<vec2> m_vSpawnPositions;

	// Timers
	float m_fPickupSpawnTimer;
	float m_fEnemySpawnTimer;
};
