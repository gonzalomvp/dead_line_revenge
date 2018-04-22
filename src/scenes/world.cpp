#include "../common/stdafx.h"
#include "../common/sys.h"
#include "../common/core.h"
#include "../common/font.h"
#include "../gui/gui.h"
#include "../scenes/world.h"
#include "../entities/entity.h"
#include "../entities/components/component.h"
#include "../entities/message.h"
#include "../globals.h"
#include "app_manager.h"
#include "level.h"
#include "../gui/string_manager.h"

struct Ball
{
	vec2   pos;
	vec2   vel;
	GLuint gfx;
	float  radius;
};

struct Bullet
{
	vec2   pos;
	vec2   vel;
	GLuint gfx;
};

struct Enemy
{
	vec2   pos;
	vec2   vel;
	GLuint gfx;
};

#define NUM_BALLS 20
Ball balls[NUM_BALLS];

#define MAX_BALL_SPEED 8.f

//-----------------------------------------------------------------------------



// Init game state
vec2 playerPos = vmake(100, 100);
vec2 targetPos = vadd(playerPos, vmake(50, 50));
vec2 targetDir = vsub(targetPos, playerPos);
vec2 bulletPos = vmake(-1, -1);
ivec2 mousePos = SYS_MousePos();
vec2 oldMousePos = vmake(mousePos.x, mousePos.y);
vec2 newMousePos = oldMousePos;

Bullet bullet;
Bullet enemybullet;
Enemy enemy;

// Load textures
GLuint texbkg;
GLuint texlargeball;
GLuint texsmallball;
GLuint targetTexture;
GLuint playerTexture;
GLuint bulletTexture;
GLuint enemyTexture;


World::World(int level) {
	m_isGameOver = false;
	m_difficulty = level;
	m_pickupSpawnWait = 10;
}

World::~World() {
	CORE_UnloadPNG(texbkg);
	CORE_UnloadPNG(texlargeball);
	CORE_UnloadPNG(texsmallball);
	CORE_UnloadPNG(targetTexture);
	for (size_t i = 0; i < m_entities.size(); ++i) {
		delete m_entities[i];
	}
	delete m_level;
}

void World::init() {
	m_isGameOver = false;
	m_pickupTimer = m_pickupSpawnWait;
	//borrar lo anterior
	for (size_t i = 0; i < m_entities.size(); ++i) {
		if (m_entities[i]) {
			delete m_entities[i];
		}
	}
	m_entities.clear();

	char *fileName = nullptr;
	switch (m_difficulty)
	{
		case 1:
			fileName = "data/level1.json";
			break;
		case 2:
			fileName = "data/level2.json";
			break;
		case 3:
			fileName = "data/level3.json";
			break;
	}

	m_player = createPlayer(vmake(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f));
	addEntity(m_player);

	//HUD
	//createHUD(m_player);

	//addEntity(createRangeEnemy(300, 300, m_player));
	bullet.pos = vmake(-1000, -1000);
	bullet.vel = vmake(0, 0);
	bullet.pos = vmake(-1000, -1000);
	bullet.vel = vmake(0, 0);
	enemy.pos = vmake(0, 0);

	m_level = Level::loadLevel(fileName);
	//m_level = new Level();
	addEntity(createTurretEnemy(100, SCR_HEIGHT - 100, vmake(0, -1), m_player));
	addEntity(createTurretEnemy(SCR_WIDTH - 100, 100, vmake(-1, 0), m_player));

	//borrar
	//addEntity(createEnemy(200, 200, m_player, 0, 1, -1));
	//addEntity(createEnemy(205, 200, m_player, 0, 1, -1));
	m_pickup = createWeaponPickup();
	addEntity(m_pickup);
	//addEntity(createWeaponPickup(vmake(400, 400), Component::ERevolver));
}

void World::addEntity(Entity* entity) {
	m_entitiesToAdd.push_back(entity);
}

void World::removeEntity(Entity* entity) {
	m_entitiesToRemove.push_back(entity);

	//m_entities.erase(std::find(m_entities.begin(), m_entities.end(), entity));
	//delete entity;
}

void World::run() {
	if (!m_isGameOver) {
		// Render
		glClear(GL_COLOR_BUFFER_BIT);

		//m_player->run();

		for (size_t i = 0; i < m_entities.size(); ++i) {
			m_entities[i]->run();
		}
		checkCollisions();
		removePendingEntities();
		addPendingEntities();
		m_level->run();

		//Pick up
		if (m_pickupTimer <= m_pickupSpawnWait) {
			++m_pickupTimer;
			if (m_pickupTimer == m_pickupSpawnWait) {
				m_pickup = createWeaponPickup();
				addEntity(m_pickup);
			}
		}
	}
}


Entity* World::getPlayer() {
	return m_player;
}

void World::checkCollisions() {
	for (size_t i = 0; i < m_entities.size(); ++i) {
		Entity* entityCollision = m_entities[i];
		MessageGetCollider msg1;
		msg1.type = Component::ENoneCollider;
		entityCollision->receiveMessage(&msg1);
		if (msg1.type != Component::ENoneCollider) {
			for (size_t j = i + 1; j < m_entities.size(); ++j) {
				Entity* entityToCheck = m_entities[j];
				MessageGetCollider msg2;
				msg2.type = Component::ENoneCollider;
				entityToCheck->receiveMessage(&msg2);
				if (msg2.type != Component::ENoneCollider && msg1.faction != msg2.faction) {
					//collision
					bool hit = false;
					switch (msg1.type)
					{
					case ComponentCollider::ECircleCollider:
						switch (msg2.type)
						{
						case ComponentCollider::ECircleCollider:
							break;
						case ComponentCollider::ERectCollider:
							hit= checkCircleRect(msg1.center, msg1.size.x * 0.5f, vsub(msg2.center, vscale(msg2.size, 0.5f)), msg2.size);
							break;
						}
						break;
					case ComponentCollider::ERectCollider:
						switch (msg2.type)
						{
						case ComponentCollider::ECircleCollider:
							hit = checkCircleRect(msg2.center, msg2.size.x * 0.5f, vsub(msg1.center, vscale(msg1.size, 0.5f)), msg1.size);
							break;
						case ComponentCollider::ERectCollider:
							hit = checkRectRect(vsub(msg1.center, vscale(msg1.size, 0.5f)), msg1.size, vsub(msg2.center, vscale(msg2.size, 0.5f)), msg2.size);
							break;
						}
						break;
					}
					if (hit) {
						MessageCollision msgCollision;
						msgCollision.deltaLife = msg1.deltaLife;
						msgCollision.other = entityCollision;
						msgCollision.faction = msg1.faction;
						entityToCheck->receiveMessage(&msgCollision);

						msgCollision.deltaLife = msg2.deltaLife;
						msgCollision.other = entityToCheck;
						msgCollision.faction = msg2.faction;
						entityCollision->receiveMessage(&msgCollision);

						//MessageLife msgLife;
						//msgLife.hit = true;

						//msgLife.lifeDelta = msg1.damage;
						//entityToCheck->receiveMessage(&msgLife);

						//msgLife.lifeDelta = msg2.damage;
						//entityCollision->receiveMessage(&msgLife);

						//removeEntity(entityCollision);
						//removeEntity(entityToCheck);
						//m_level->killEnemy();
						//if(entityCollision == m_player || entityToCheck == m_player)
						//	g_appManager->switchMode(ModeId::MODE_MENU);
						//else
						//	addEntity(createEnemy(300, 300, m_player));
						
						//ver si arregla colisiones multiples
						//break;
					}
				}
			}
		}
	}
}

void World::removePendingEntities() {
	for (auto it = m_entitiesToRemove.begin(); it != m_entitiesToRemove.end(); ++it)
	{
		m_level->killEnemy(*it);
		auto it2 = m_entities.begin();
		bool bErased = false;
		while (!bErased && (it2 != m_entities.end()))
		{
			if (*it == *it2)
			{
				delete *it2;
				m_entities.erase(it2);
				bErased = true;
			}
			else
				++it2;
		}


		//Poner antes
		if (*it == m_player) {
			m_isGameOver = true;
			std::string scoreMessage = g_stringManager->getText("LTEXT_GUI_SCORE_MESSAGE") + std::to_string(m_level->m_score);
			g_menuManager->getMenu(MenuManager::EGameOverMenu)->setTitle(scoreMessage.c_str());
			g_menuManager->activateMenu(MenuManager::EGameOverMenu);
			for (size_t i = 0; i < m_entities.size(); ++i) {
				delete m_entities[i];
			}
			m_entities.clear();
			m_player = nullptr;
		}
		else if (*it == m_pickup) {
			m_pickupTimer = 0;
			m_pickup = nullptr;
		}
	}
	m_entitiesToRemove.clear();
}

void World::addPendingEntities() {
	for (size_t i = 0; i < m_entitiesToAdd.size(); ++i) {
		m_entities.push_back(m_entitiesToAdd[i]);
	}
	m_entitiesToAdd.clear();
}

Entity* createPlayer(vec2 pos) {
	Entity* player = new Entity();
	ComponentTransform* transform = new ComponentTransform(player, pos, vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(player, "data/player.png", 1.0f, "data/enemy.png", 10);
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

Entity* createBullet(vec2 pos, vec2 direction, float speed, int damage, int range, ComponentCollider::TFaction faction) {
	Entity* bullet = new Entity();
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

void createShotgunBullets(vec2 pos, vec2 direction, float speed, int damage, int range, ComponentCollider::TFaction faction) {
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

Entity* createMine(Component* weapon, vec2 pos, int damage, ComponentCollider::TFaction faction) {
	Entity* mine = new Entity();
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

Entity* createC4(Component* weapon, vec2 pos, int damage, ComponentCollider::TFaction faction) {
	Entity* mine = new Entity();
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

Entity* createRocket(Component* weapon, vec2 pos, vec2 direction, float speed, int damage, int range, ComponentCollider::TFaction faction) {
	Entity* rocket = new Entity();
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

Entity* createNuclearBomb() {
	Entity* bomb = new Entity();
	ComponentTransform* transform = new ComponentTransform(bomb, vmake(SCR_WIDTH / 2.0, SCR_HEIGHT/ 2.0), vmake(SCR_WIDTH, SCR_HEIGHT));
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

Entity* createEnemy(int x, int y, Entity* player, int speed, int lives, int damage) {
	Entity* enemy = new Entity();
	ComponentTransform* transform = new ComponentTransform(enemy, vmake(x, y), vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", 1.0f, "data/player.png", 10);
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

Entity* createBigEnemy(int x, int y, Entity* player, int speed, int lives, int damage) {
	Entity* enemy = new Entity();
	ComponentTransform* transform = new ComponentTransform(enemy, vmake(x, y), vmake(30, 30));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", 1.0f, "data/player.png", 10);
	renderable->init();
	ComponentAIMelee* ai = new ComponentAIMelee(enemy, player, speed, 0);
	ai->init();
	ComponentCollider* collider = new ComponentCollider(enemy, ComponentCollider::ERectCollider, ComponentCollider::EEnemy, damage);
	collider->init();
	ComponentLife* life = new ComponentLife(enemy, lives, 0, 0);
	life->init();
	return enemy;
}

Entity* createRangeEnemy(int x, int y, Entity* player) {
	Entity* enemy = new Entity();
	ComponentTransform* transform = new ComponentTransform(enemy, vmake(x, y), vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", 1.0f, "data/player.png", 10);
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

Entity* createTurretEnemy(int x, int y, vec2 dir, Entity* player) {
	Entity* enemy = new Entity();
	ComponentTransform* transform = new ComponentTransform(enemy, vmake(x, y), vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", 1.0f, "data/player.png", 10);
	renderable->init();
	ComponentInertialMove* movement = new ComponentInertialMove(enemy, vmake(1,0), 2, true);
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

Entity* createWeaponPickup() {
	int randomType = rand() % Component::EWeaponCount;
	vec2 randomPos = vmake(CORE_FRand(0.0, SCR_WIDTH), CORE_FRand(0.0, SCR_HEIGHT));
	Entity* weaponPickup = new Entity();
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

Entity* createHUDMessage(std::string message, vec2 pos, int displayTime) {
	Entity* hudMessage = new Entity();
	ComponentHUDMessage* hudMessageComponent = new ComponentHUDMessage(hudMessage, pos, message);
	hudMessageComponent->init();
	ComponentLife* life = new ComponentLife(hudMessage, 0, displayTime, 0);
	life->init();
	if (g_world->m_hudMessage) {

		//mal puede apuntar a otro sitio
		g_world->removeEntity(g_world->m_hudMessage);
	}
	g_world->m_hudMessage = hudMessage;
	g_world->addEntity(hudMessage);
	return hudMessage;
}

void createExplossion(vec2 pos, vec2 size) {
	Entity* explossionFx = new Entity();
	ComponentTransform* transform = new ComponentTransform(explossionFx, pos, vmake(10,10), vmake(2, 2));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(explossionFx, "data/bullet.png", 0.5f);
	renderable->init();
	ComponentLife* life = new ComponentLife(explossionFx, -1, 50, 0);
	life->init();
	g_world->addEntity(explossionFx);

	Entity* explossionImpactToEnemies = new Entity();
	transform = new ComponentTransform(explossionImpactToEnemies, pos, vmake(10, 10), vmake(2, 2));
	transform->init();
	ComponentCollider* colliderEnemy = new ComponentCollider(explossionImpactToEnemies, ComponentCollider::ECircleCollider, ComponentCollider::EAllied, -5);
	colliderEnemy->init();
	life = new ComponentLife(explossionImpactToEnemies, -1, 50, 0);
	life->init();
	g_world->addEntity(explossionImpactToEnemies);

	Entity* explossionImpactToPlayer = new Entity();
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

bool checkCircleRect(const vec2& circlePos, float circleRadius, const vec2& rectPos, const vec2& rectSize) {
	vec2 closestPoint = vmake(clamp(circlePos.x, rectPos.x, rectPos.x + rectSize.x), clamp(circlePos.y, rectPos.y, rectPos.y + rectSize.y));
	return vdist(circlePos, closestPoint) < circleRadius;
}

bool checkRectRect(const vec2& rectPos1, const vec2& rectSize1, const vec2& rectPos2, const vec2& rectSize2) {
	return ((isInBounds(rectPos1.x, rectPos2.x, rectPos2.x + rectSize2.x) || isInBounds(rectPos2.x, rectPos1.x, rectPos1.x + rectSize1.x) || rectPos1.x == rectPos2.x) &&
		(isInBounds(rectPos1.y, rectPos2.y, rectPos2.y + rectSize2.y) || isInBounds(rectPos2.y, rectPos1.y, rectPos1.y + rectSize1.y) || rectPos1.y == rectPos2.y));
}