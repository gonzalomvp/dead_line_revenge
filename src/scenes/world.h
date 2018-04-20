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

	void init();
	void run();

	Entity* getPlayer();

	Entity* m_player;
	Entity* m_pickup;
	int m_pickupTimer;
	int m_pickupSpawnWait;
	std::vector <Entity*> m_entities;
	Level* m_level;
	int m_difficulty;

private:
	void checkCollisions();
	void removePendingEntities();

	std::vector <Entity*> m_entitiesToRemove;
	bool m_isGameOver;

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
Entity* createBullet(vec2 pos, vec2 direction, float speed, int damage, int range, ComponentCollider::TFaction faction);
void createShotgunBullets(vec2 pos, vec2 direction, float speed, int damage, int range, ComponentCollider::TFaction faction);
Entity* createWeaponPickup(vec2 pos, Component::TWeapon type);
Entity* createHUDMessage(std::string, vec2 pos, int displayTime);

// Collision utils
inline float clamp(float value, float inf, float sup) { return (value <= inf) ? inf : (value >= sup) ? sup : value; };
inline bool  isInBounds(float value, float inf, float sup) { return (value > inf) && (value < sup); };
bool checkCircleCircle(const vec2& pos1, float radius1, const vec2& pos2, float radius2);
bool checkCircleRect(const vec2& circlePos, float circleRadius, const vec2& rectPos, const vec2& rectSize);
bool checkRectRect(const vec2& rectPos1, const vec2& rectSize1, const vec2& rectPos2, const vec2& rectSize2);