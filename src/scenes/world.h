#pragma once
//#include <vector>
#include "../entities/entity.h"
#include "../entities/components/component.h"

class Level;

class World : public IInputManager::IListener {
public:
	World(int level);
	~World();
	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);

	void init();
	void run();
	virtual bool onEvent(const IInputManager::Event& event);

	Entity* getPlayer();

	Entity* m_player;
	Entity* m_pickup;
	int m_pickupTimer;
	int m_pickupSpawnWait;
	std::vector <Entity*> m_entities;
	Level* m_level;
	int m_difficulty;
	Entity* m_hudMessage;
private:
	void checkCollisions();
	void removePendingEntities();
	void addPendingEntities();

	std::vector <Entity*> m_entitiesToRemove;
	std::vector <Entity*> m_entitiesToAdd;
	bool m_isGameOver;
	bool     m_isPaused;

	//HUD
	
};

// Collision utils
inline float clamp(float value, float inf, float sup) { return (value <= inf) ? inf : (value >= sup) ? sup : value; };
inline bool  isInBounds(float value, float inf, float sup) { return (value > inf) && (value < sup); };
bool checkCircleCircle(const vec2& pos1, float radius1, const vec2& pos2, float radius2);
bool checkCircleRect(const vec2& circlePos, float circleRadius, const vec2& rectPos, const vec2& rectSize);
bool checkRectRect(const vec2& rectPos1, const vec2& rectSize1, const vec2& rectPos2, const vec2& rectSize2);