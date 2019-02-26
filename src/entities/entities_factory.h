#pragma once

#include "entity.h"

class EntitiesFactory {
public:
	static Entity* createEntity(Entity::TType type);
	static Entity* createEnemyMelee();
	static Entity* createEnemyBig();
	static Entity* createEnemyRange();
	static Entity* createEnemyTurret();
	static Entity* createPlayer();
	static Entity* createBullet();
};