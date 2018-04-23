#include "../common/stdafx.h"
#include "entity.h"

#include "../scenes/world.h"

Entity::~Entity() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		delete *itComponents;
	}
}

void Entity::activate() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->activate();
	}
}

void Entity::deactivate() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->deactivate();
	}
}

void Entity::run() {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->run();
	}
}

void Entity::addComponent(Component* component) {
	m_components.push_back(component);
}

void Entity::receiveMessage(Message* message) {
	for (auto itComponents = m_components.begin(); itComponents != m_components.end(); ++itComponents) {
		(*itComponents)->receiveMessage(message);
	}
}

Entity* Entity::createPlayer(vec2 pos) {
	Entity* player = new Entity(EPlayer);
	ComponentTransform* transform = new ComponentTransform(player, pos, vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(player, "data/player.png", 2, 1.0f, "data/enemy.png", 10);
	renderable->init();
	ComponentPlayerController* playerControl = new ComponentPlayerController(player, 5);
	playerControl->init();
	ComponentInertialMove* movement = new ComponentInertialMove(player, vmake(0.0f, 0.0f), 5, false);
	movement->init();
	ComponentWeapon* weapon = new ComponentWeapon(player, Component::ERevolver, 20, 60, 6, 6, -2, 60, false, "data/shot.wav");
	weapon->init();
	C_Target* target = new C_Target(player, "data/target.png");
	target->init();
	ComponentCollider* collider = new ComponentCollider(player, ComponentCollider::ERectCollider, ComponentCollider::EAllied, 0);
	collider->init();
	ComponentLife* life = new ComponentLife(player, 5, 0, 20);
	life->init();
	ComponentHUD* hudComponent = new ComponentHUD(player);
	hudComponent->init();
	return player;
}

Entity* Entity::createBullet(vec2 pos, vec2 direction, float speed, int damage, int range, ComponentCollider::TFaction faction) {
	Entity* bullet = new Entity(EWeapon);
	ComponentTransform* transform = new ComponentTransform(bullet, pos, vmake(10, 10));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(bullet, "data/bullet.png");
	renderable->init();
	ComponentInertialMove* movement = new ComponentInertialMove(bullet, direction, speed, true);
	movement->init();
	ComponentCollider* collider = new ComponentCollider(bullet, ComponentCollider::ECircleCollider, faction, damage);
	collider->init();
	ComponentLife* life = new ComponentLife(bullet, 0, range, 0);
	life->init();
	return bullet;
}

void Entity::createShotgunBullets(vec2 pos, vec2 direction, float speed, int damage, int range, ComponentCollider::TFaction faction) {
	float dispersionAngle = 15.0f;

	vec2 bulletDir = direction;
	g_world->addEntity(createBullet(pos, bulletDir, speed, damage, range, faction));

	float angle = vangle(direction);
	angle += dispersionAngle;
	bulletDir = vunit(DEG2RAD(angle));
	g_world->addEntity(createBullet(pos, bulletDir, speed, damage, range, faction));

	angle = vangle(direction);
	angle -= dispersionAngle;
	bulletDir = vunit(DEG2RAD(angle));
	g_world->addEntity(createBullet(pos, bulletDir, speed, damage, range, faction));

	return;
}

Entity* Entity::createMine(Component* weapon, vec2 pos, int damage, ComponentCollider::TFaction faction) {
	Entity* mine = new Entity(EWeapon);
	ComponentTransform* transform = new ComponentTransform(mine, pos, vmake(10, 10));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(mine, "data/bullet.png");
	renderable->init();
	ComponentCollider* collider = new ComponentCollider(mine, ComponentCollider::ECircleCollider, faction, 0);
	collider->init();
	ComponentLife* life = new ComponentLife(mine, 0, 0, 0);
	life->init();
	ComponentExplossive* explossion = new ComponentExplossive(mine, false, weapon);
	explossion->init();
	g_world->addEntity(mine);
	return mine;
}

Entity* Entity::createC4(Component* weapon, vec2 pos, int damage, ComponentCollider::TFaction faction) {
	Entity* mine = new Entity(EWeapon);
	ComponentTransform* transform = new ComponentTransform(mine, pos, vmake(10, 10));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(mine, "data/bullet.png");
	renderable->init();
	ComponentLife* life = new ComponentLife(mine, 0, 0, 0);
	life->init();
	ComponentExplossive* explossion = new ComponentExplossive(mine, true, weapon);
	explossion->init();
	g_world->addEntity(mine);
	return mine;
}

Entity* Entity::createRocket(Component* weapon, vec2 pos, vec2 direction, float speed, int damage, int range, ComponentCollider::TFaction faction) {
	Entity* rocket = new Entity(EWeapon);
	ComponentTransform* transform = new ComponentTransform(rocket, pos, vmake(10, 10));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(rocket, "data/bullet.png");
	renderable->init();
	ComponentInertialMove* movement = new ComponentInertialMove(rocket, direction, speed, true);
	movement->init();
	ComponentCollider* collider = new ComponentCollider(rocket, ComponentCollider::ECircleCollider, faction, damage);
	collider->init();
	ComponentLife* life = new ComponentLife(rocket, 0, range, 0);
	life->init();
	ComponentExplossive* explossive = new ComponentExplossive(rocket, false, weapon);
	explossive->init();
	g_world->addEntity(rocket);
	return rocket;
}

Entity* Entity::createNuclearBomb() {
	Entity* bomb = new Entity(EWeapon);
	ComponentTransform* transform = new ComponentTransform(bomb, vmake(SCR_WIDTH / 2.0, SCR_HEIGHT / 2.0), vmake(SCR_WIDTH, SCR_HEIGHT));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(bomb, "data/bullet.png", 0.5f);
	renderable->init();
	ComponentCollider* colliderEnemy = new ComponentCollider(bomb, ComponentCollider::ECircleCollider, ComponentCollider::EAllied, -50);
	colliderEnemy->init();
	ComponentLife* life = new ComponentLife(bomb, -1, 50, 0);
	life->init();
	g_world->addEntity(bomb);

	if (g_settings.sfx) {
		uint m_soundId = CORE_LoadWav("data/explossion.wav");
		CORE_PlayMusic(m_soundId);
	}

	return bomb;
}

Entity* Entity::createEnemy(int x, int y, Entity* player, int speed, int lives, int damage) {
	Entity* enemy = new Entity(EEnemy);
	ComponentTransform* transform = new ComponentTransform(enemy, vmake(x, y), vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", 2, 1.0f, "data/player.png", 10);
	renderable->init();
	ComponentAIMelee* ai = new ComponentAIMelee(enemy, player, speed, 0);
	ai->init();
	ComponentCollider* collider = new ComponentCollider(enemy, ComponentCollider::ERectCollider, ComponentCollider::EEnemy, damage);
	collider->init();
	ComponentLife* life = new ComponentLife(enemy, lives, 0, 0);
	life->init();
	ComponentPoints* points = new ComponentPoints(enemy, 10);
	points->init();
	return enemy;
}

Entity* Entity::createBigEnemy(int x, int y, Entity* player, int speed, int lives, int damage) {
	Entity* enemy = new Entity(EEnemy);
	ComponentTransform* transform = new ComponentTransform(enemy, vmake(x, y), vmake(30, 30));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", 2, 1.0f, "data/player.png", 10);
	renderable->init();
	ComponentAIMelee* ai = new ComponentAIMelee(enemy, player, speed, 0);
	ai->init();
	ComponentCollider* collider = new ComponentCollider(enemy, ComponentCollider::ERectCollider, ComponentCollider::EEnemy, damage);
	collider->init();
	ComponentLife* life = new ComponentLife(enemy, lives, 0, 0);
	life->init();
	return enemy;
}

Entity* Entity::createRangeEnemy(int x, int y, Entity* player) {
	Entity* enemy = new Entity(EEnemy);
	ComponentTransform* transform = new ComponentTransform(enemy, vmake(x, y), vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", 2, 1.0f, "data/player.png", 10);
	renderable->init();
	ComponentWeapon* gun = new ComponentWeapon(enemy, Component::ERevolver, 40, 1, -1, 6, -1, 0, true);
	gun->init();
	ComponentAIEvade* aiLong = new ComponentAIEvade(enemy, player, 4, 250);
	aiLong->init();
	ComponentAIMelee* aiMelee = new ComponentAIMelee(enemy, player, 4, 300);
	aiMelee->init();
	ComponentAIFire* aiFire = new ComponentAIFire(enemy, player);
	aiFire->init();
	ComponentCollider* collider = new ComponentCollider(enemy, ComponentCollider::ERectCollider, ComponentCollider::EEnemy, -1);
	collider->init();
	ComponentLife* life = new ComponentLife(enemy, 1, 0, 0);
	life->init();
	return enemy;
}

Entity* Entity::createTurretEnemy(int x, int y, vec2 dir, Entity* player) {
	Entity* enemy = new Entity(EOther);
	ComponentTransform* transform = new ComponentTransform(enemy, vmake(x, y), vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", 2, 1.0f, "data/player.png", 10);
	renderable->init();
	ComponentInertialMove* movement = new ComponentInertialMove(enemy, vmake(1, 0), 2, true);
	movement->init();
	ComponentWeapon* gun = new ComponentWeapon(enemy, Component::ERevolver, 100, 1, -1, 6, -1, 0, true);
	gun->init();
	std::vector<vec2> aimDirections = { vmake(1,0), vmake(0,1), vmake(-1, 0), vmake(0, -1) };
	ComponentAIFire* aiFire = new ComponentAIFire(enemy, aimDirections);
	aiFire->init();
	ComponentCollider* collider = new ComponentCollider(enemy, ComponentCollider::ERectCollider, ComponentCollider::EEnemy, -1);
	collider->init();
	ComponentLife* life = new ComponentLife(enemy, 3, 0, 0);
	life->init();
	ComponentPoints* points = new ComponentPoints(enemy, 10);
	points->init();
	return enemy;
}

Entity* Entity::createWeaponPickup() {
	int randomType = rand() % Component::EWeaponCount;
	vec2 randomPos = vmake(CORE_FRand(0.0, SCR_WIDTH), CORE_FRand(0.0, SCR_HEIGHT));
	Entity* weaponPickup = new Entity(EPickup);
	ComponentTransform* transform = new ComponentTransform(weaponPickup, randomPos, vmake(20, 20));
	transform->init();

	Component::TWeapon type;
	switch (randomType)
	{
		case 0:
			type = Component::ERevolver;
			break;
		case 1:
			type = Component::EMachinegun;
			break;
		case 2:
			type = Component::EShotgun;
			break;
		case 3:
			type = Component::EMines;
			break;
		case 4:
			type = Component::EC4;
			break;
		case 5:
			type = Component::ERocketLauncher;
			break;
	}
	ComponentRenderable* renderable = new ComponentRenderable(weaponPickup, "data/SimpleCrate.png");
	renderable->init();
	ComponentCollider* collider = new ComponentCollider(weaponPickup, ComponentCollider::ERectCollider, ComponentCollider::ENeutral, 0);
	collider->init();
	ComponentWeaponPickup* pickup = new ComponentWeaponPickup(weaponPickup, type);
	pickup->init();
	ComponentPoints* points = new ComponentPoints(weaponPickup, 1);
	points->init();
	ComponentLife* life = new ComponentLife(weaponPickup, 0, 0, 0);
	life->init();
	return weaponPickup;
}

Entity* Entity::createHUDMessage(std::string message, vec2 pos, int displayTime) {
	Entity* hudMessage = new Entity(EHUD);
	ComponentHUDMessage* hudMessageComponent = new ComponentHUDMessage(hudMessage, pos, message);
	hudMessageComponent->init();
	ComponentLife* life = new ComponentLife(hudMessage, 0, displayTime, 0);
	life->init();
	//if (g_world->m_hudMessage) {

	//	//mal puede apuntar a otro sitio
	//	g_world->removeEntity(g_world->m_hudMessage);
	//}
	//g_world->m_hudMessage = hudMessage;
	g_world->addEntity(hudMessage);
	return hudMessage;
}

void Entity::createExplossion(vec2 pos, vec2 size) {
	Entity* explossionFx = new Entity(EWeapon);
	ComponentTransform* transform = new ComponentTransform(explossionFx, pos, vmake(10, 10), vmake(2, 2));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(explossionFx, "data/bullet.png", 2, 0.5f);
	renderable->init();
	ComponentLife* life = new ComponentLife(explossionFx, -1, 50, 0);
	life->init();
	g_world->addEntity(explossionFx);

	Entity* explossionImpactToEnemies = new Entity(EWeapon);
	transform = new ComponentTransform(explossionImpactToEnemies, pos, vmake(10, 10), vmake(2, 2));
	transform->init();
	ComponentCollider* colliderEnemy = new ComponentCollider(explossionImpactToEnemies, ComponentCollider::ECircleCollider, ComponentCollider::EAllied, -5);
	colliderEnemy->init();
	life = new ComponentLife(explossionImpactToEnemies, -1, 50, 0);
	life->init();
	g_world->addEntity(explossionImpactToEnemies);

	Entity* explossionImpactToPlayer = new Entity(EWeapon);
	transform = new ComponentTransform(explossionImpactToPlayer, pos, vmake(10, 10), vmake(2, 2));
	transform->init();
	ComponentCollider* colliderAllied = new ComponentCollider(explossionImpactToPlayer, ComponentCollider::ECircleCollider, ComponentCollider::EEnemy, -1);
	colliderAllied->init();
	life = new ComponentLife(explossionImpactToPlayer, -1, 50, 0);
	life->init();
	g_world->addEntity(explossionImpactToPlayer);

	if (g_settings.sfx) {
		uint m_soundId = CORE_LoadWav("data/explossion.wav");
		CORE_PlayMusic(m_soundId);
	}

	return;
}