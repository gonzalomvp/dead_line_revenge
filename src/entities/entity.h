#pragma once

#include "components/component.h"

struct Message;

class Entity {
public:
	enum TType {
		EPlayer,
		EPickup,
		EEnemy,
		EWeapon,
		EHUD,
		EOther,
	};

	static Entity* createPlayer(vec2 pos);
	static Entity* createEnemy(int x, int y, Entity* player, int speed, int lives, int damage);
	static Entity* createBigEnemy(int x, int y, Entity* player, int speed, int lives, int damage);
	static Entity* createRangeEnemy(int x, int y, Entity* player);
	static Entity* createTurretEnemy(int x, int y, vec2 dir, Entity* player);
	static Entity* createBullet(vec2 pos, vec2 direction, float speed, int damage, int range, Entity::TType type);
	static void createShotgunBullets(vec2 pos, vec2 direction, float speed, int damage, int range, Entity::TType type);
	static Entity* createMine(Component* weapon, vec2 pos, int damage, Entity::TType type);
	static Entity* createC4(Component* weapon, vec2 pos, int damage, Entity::TType type);
	static Entity* createRocket(Component* weapon, vec2 pos, vec2 direction, float speed, int damage, int range, Entity::TType type);
	static Entity* createNuclearBomb();
	static Entity* createWeaponPickup();
	static void createExplossion(vec2 pos, vec2 size);
	static Entity* createHUDMessage(std::string, vec2 pos, int displayTime);

	Entity(TType type) : m_type(type) {}
	~Entity();

	void activate();
	void deactivate();
	TType getType() { return m_type; }

	void run(float deltaTime);
	void receiveMessage(Message* message);
	void addComponent(Component* component);
	void removeComponent(Component* component) {}

	std::vector<Component*> m_components;
private:
	TType m_type;
};

// Game utils
