#pragma once
//#include <vector>
#include "../entities/entity.h"
#include "../entities/components/component.h"

class Level;

class World {
public:
	World(int level);
	~World();
	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);

	void run();

	Entity* getPlayer();

	Entity* m_player;
	std::vector <Entity*> m_entities;
	Level* m_level;

private:
	void checkCollisions();
	void removePendingEntities();

	std::vector <Entity*> m_entitiesToRemove;
	

	//HUD
	Text* m_lifeHUD;
	Text* m_scoreHUD;
	Text* m_ammoHUD;
	Sprite* m_reloadAnim;
};


// Game utils
Entity* createPlayer(vec2 pos);
Entity* createEnemy(int x, int y, Entity* player, int speed, int lives, int damage);
Entity* createBigEnemy(int x, int y, Entity* player, int speed, int lives, int damage);
Entity* createRangeEnemy(int x, int y, Entity* player);
Entity* createTurretEnemy(int x, int y, vec2 dir, Entity* player);
Entity* createBullet(vec2 pos, vec2 direction, float speed, Component::TFaction faction);
Entity* createWeaponPickup(vec2 pos, Component::TWeapon type);

// Collision utils
inline float clamp(float value, float inf, float sup) { return (value <= inf) ? inf : (value >= sup) ? sup : value; };
inline bool  isInBounds(float value, float inf, float sup) { return (value > inf) && (value < sup); };
bool checkCircleCircle(const vec2& pos1, float radius1, const vec2& pos2, float radius2);
bool checkCircleRect(const vec2& circlePos, float circleRadius, const vec2& rectPos, const vec2& rectSize);
bool checkRectRect(const vec2& rectPos1, const vec2& rectSize1, const vec2& rectPos2, const vec2& rectSize2);