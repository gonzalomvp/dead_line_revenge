#pragma once

#include "entities/Entity.h"
#include "input/IInputManager.h"

#define WORLD_WIDTH  640
#define WORLD_HEIGHT 480

class CWorld : public IInputManager::IListener {
public:
	CWorld();
	~CWorld();

	bool         init           (uint16_t _uLevel);
	void         cleanup        ();
	void         run            (float _fDeltaTime);
	void         addEntity      (CEntity* _pEntity);
	void         removeEntity   (CEntity* _pEntity);
	uint16_t     getLevel       () const                      { return m_uLevel;                           }
	uint16_t     getScore       () const                      { return m_uScore;                           }
	CEntity*     getPlayer      () const                      { return m_pPlayer;                          }
	int	         getPlayerLife  () const                      { return m_iPlayerLife;                      }
	float	     getPlayerSpeed () const                      { return m_fPlayerSpeed;                     }
	void         addPoints      (CEntity::EType _tEntityType) { m_uScore += m_mEntityPoints[_tEntityType]; }

	// IInputManager::IListener
	virtual bool onEvent (const IInputManager::CEvent& _event);

private:
	void checkCollisions      ();
	void removePendingEntities();
	void addPendingEntities   ();
	void spawnNewEntities     (float _fDeltaTime);

	uint16_t               m_uLevel;
	CEntity*               m_pPlayer;
	std::vector <CEntity*> m_vEntities;
	std::vector <CEntity*> m_vEntitiesToRemove;
	std::vector <CEntity*> m_vEntitiesToAdd;
	bool                   m_bIsGameOver;
	bool                   m_bIsPaused;

	// Game rules
	uint16_t                           m_uScore;
	int                                m_iPlayerLife;
	float                              m_fPlayerSpeed;
	float                              m_fPickupSpawnWait;
	float                              m_fEnemySpawnWait;
	int                                m_iCurrentEnemies;
	int                                m_iMaxEnemies;
	int                                m_iBossSpawnPoints;
	int                                m_iBossSpawnPointsCounter;
	std::map<CEntity::EType, float>    m_mEnemyProbabilities;
	std::map<CEntity::EType, uint16_t> m_mEntityPoints;
	std::vector<vec2>                  m_vSpawnPositions;

	// Timers
	float m_fPickupSpawnTimer;
	float m_fEnemySpawnTimer;
};
