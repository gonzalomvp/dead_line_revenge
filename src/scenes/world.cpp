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

	char *fileName = nullptr;
	switch (level)
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
	addEntity(createWeaponPickup(vmake(250, 250), Component::EMachinegun));
	//addEntity(createWeaponPickup(vmake(400, 400), Component::ERevolver));

	//HUD
	m_lifeHUD = new Text("", 1,  vmake(20, 20));
	//g_graphicsEngine->addGfxEntity(m_lifeHUD);
	Entity* hudLife = new Entity();
	C_HUDLife* c_hudLife = new C_HUDLife(hudLife, m_player);
	c_hudLife->init();
	addEntity(hudLife);


	m_scoreHUD = new Text("| 120", 1, vmake(40, 20));
	g_graphicsEngine->addGfxEntity(m_scoreHUD);

	m_ammoHUD = new Text("| 6/-", 1, vmake(120, 20));
	g_graphicsEngine->addGfxEntity(m_ammoHUD);

	//ver si se puede hacer de otra forma
	m_reloadAnim = new Sprite(g_graphicsEngine->getTexture("data/playerReload.png"), 1);
	g_graphicsEngine->addGfxEntity(m_reloadAnim);

	//for (int i = 0; i < NUM_BALLS; i++)
	//{
	//  balls[i].pos = vmake(CORE_FRand(0.0, SCR_WIDTH), CORE_FRand(0.0, SCR_HEIGHT));
	//  balls[i].vel = vmake(CORE_FRand(-MAX_BALL_SPEED, +MAX_BALL_SPEED), CORE_FRand(-MAX_BALL_SPEED, +MAX_BALL_SPEED));
	//  if (CORE_FRand(0.f, 1.f) < 0.10f)
	//  {
	//    balls[i].radius = 64.f;
	//    balls[i].gfx = texlargeball;
	//  }
	//  else
	//  {
	//    balls[i].radius = 16.f;
	//    balls[i].gfx = texsmallball;
	//  }
	//}
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

	g_graphicsEngine->removeGfxEntity(m_lifeHUD);
	g_graphicsEngine->removeGfxEntity(m_scoreHUD);
	g_graphicsEngine->removeGfxEntity(m_ammoHUD);
	g_graphicsEngine->removeGfxEntity(m_reloadAnim);
	delete m_lifeHUD;
	delete m_scoreHUD;
	delete m_ammoHUD;
	delete m_reloadAnim;
}

void World::addEntity(Entity* entity) {
	m_entities.push_back(entity);
}

void World::removeEntity(Entity* entity) {
	m_entitiesToRemove.push_back(entity);

	//m_entities.erase(std::find(m_entities.begin(), m_entities.end(), entity));
	//delete entity;
}

void World::run() {
	// Render
	glClear(GL_COLOR_BUFFER_BIT);

	//m_player->run();

	for (size_t i = 0; i < m_entities.size(); ++i) {
		m_entities[i]->run();
	}
	checkCollisions();
	removePendingEntities();
	m_level->run();

	//HUD
	if (m_player) {
		m_scoreHUD->setText("- " + std::to_string(m_level->m_score));

		MessageGetLife msgLife;
		m_player->receiveMessage(&msgLife);
		m_lifeHUD->setText(std::to_string(msgLife.currentLife));

		MessageAmmoInfo msgAmmo;
		m_player->receiveMessage(&msgAmmo);
		std::string totalAmmo = "-";
		if (msgAmmo.totalAmmo >= 0)
			totalAmmo = std::to_string(msgAmmo.totalAmmo);
		m_ammoHUD->setText("- " + std::to_string(msgAmmo.currentAmmo) + "/" + totalAmmo);

		MessageGetTransform msgTransform;
		m_player->receiveMessage(&msgTransform);
		m_reloadAnim->setPos(vmake(msgTransform.pos.x, msgTransform.pos.y - msgTransform.size.y * msgAmmo.reloadPercent * 0.5f));
		m_reloadAnim->setSize(vmake(msgTransform.size.x, msgTransform.size.y * (1- msgAmmo.reloadPercent)));
	}
	

	//ivec2 mousePos = SYS_MousePos();
	//newMousePos = vmake(mousePos.x, mousePos.y);

	//vec2 dirMouseMove = vsub(newMousePos, playerPos);
	//if (dirMouseMove.x != 0 || dirMouseMove.y != 0) {
	//	targetDir = vscale(vscale(dirMouseMove, 1.0f / vlen(dirMouseMove)), 100);
	//}

	//targetPos = vadd(playerPos, targetDir);

	//oldMousePos = newMousePos;

	//CORE_RenderCenteredSprite(targetPos, vmake(50, 50), targetTexture);
	//CORE_RenderCenteredSprite(playerPos, vmake(20, 20), playerTexture);
	//CORE_RenderCenteredSprite(bullet.pos, vmake(10, 10), bulletTexture);
	//CORE_RenderCenteredSprite(enemy.pos, vmake(20, 20), enemyTexture);


	//SYS_Show();

	//// Run balls
	//bullet.pos = vadd(bullet.pos, bullet.vel);
	//vec2 enemyDir = vsub(playerPos, enemy.pos);
	//enemy.pos = vadd(enemy.pos, vscale(enemyDir, 0.01f));

	//if (vlen(vsub(bullet.pos, enemy.pos)) < 10) {
	//	enemy.pos = vmake(0, 0);
	//	bullet.pos = vmake(-1000, -1000);
	//	bullet.vel = vmake(0, 0);
	//}

	//if (SYS_KeyPressed(SYS_KEY_LEFT)) {
	//	playerPos = vadd(playerPos, vmake(-5, 0));
	//}
	//if (SYS_KeyPressed(SYS_KEY_RIGHT)) {
	//	playerPos = vadd(playerPos, vmake(5, 0));
	//}
	//if (SYS_KeyPressed(SYS_KEY_UP)) {
	//	playerPos = vadd(playerPos, vmake(0, 5));
	//}
	//if (SYS_KeyPressed(SYS_KEY_DOWN)) {
	//	playerPos = vadd(playerPos, vmake(0, -5));
	//}

	//if (SYS_MouseButonPressed(SYS_MB_LEFT)) {
	//	bullet.pos = vadd(playerPos, vscale(targetDir, 0.2f));
	//	bullet.vel = vscale(targetDir, 0.1f);
	//}
	//if (SYS_MouseButonPressed(SYS_MB_RIGHT)) {
	//	enemybullet.pos = enemy.pos;
	//	enemybullet.vel = vscale(targetDir, 0.1f);
	//}

	//for (int i = 0; i < NUM_BALLS; i++)
	//{
	//	vec2 oldpos = balls[i].pos;
	//	vec2 newpos = vadd(oldpos, balls[i].vel);

	//	bool collision = false;
	//	int colliding_ball = -1;
	//	for (int j = 0; j < NUM_BALLS; j++)
	//	{
	//		if (i != j)
	//		{
	//			float limit2 = (balls[i].radius + balls[j].radius) * (balls[i].radius + balls[j].radius);
	//			if (vlen2(vsub(oldpos, balls[j].pos)) > limit2 && vlen2(vsub(newpos, balls[j].pos)) <= limit2)
	//			{
	//				collision = true;
	//				colliding_ball = j;
	//				break;
	//			}
	//		}
	//	}

	//	if (!collision)
	//		balls[i].pos = newpos;
	//	else
	//	{
	//		// Rebound!
	//		balls[i].vel = vscale(balls[i].vel, -1.f);
	//		balls[colliding_ball].vel = vscale(balls[colliding_ball].vel, -1.f);
	//	}

	//	// Rebound on margins
	//	if (balls[i].vel.x > 0.0)
	//	{
	//		if (balls[i].pos.x > SCR_WIDTH)
	//			balls[i].vel.x *= -1.0;
	//	}
	//	else {
	//		if (balls[i].pos.x < 0)
	//			balls[i].vel.x *= -1.0;
	//	}
	//	if (balls[i].vel.y > 0.0)
	//	{
	//		if (balls[i].pos.y > SCR_HEIGHT)
	//			balls[i].vel.y *= -1.0;
	//	}
	//	else {
	//		if (balls[i].pos.y < 0)
	//			balls[i].vel.y *= -1.0;
	//	}
	//}
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
						break;
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

		if (*it == m_player) {
			g_appManager->switchMode(ModeId::MODE_MENU);
			m_player = nullptr;
		}
	}
	m_entitiesToRemove.clear();
}

Entity* createPlayer(vec2 pos) {
	Entity* player = new Entity();
	ComponentTransform* transform = new ComponentTransform(player, pos, vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(player, "data/player.png", "data/enemy.png", 10);
	renderable->init();
	ComponentPlayerController* playerControl = new ComponentPlayerController(player, 5);
	playerControl->init();
	ComponentInertialMove* movement = new ComponentInertialMove(player, vmake(0.0f, 0.0f), 5, false);
	movement->init();
	ComponentWeapon* revolver = new ComponentWeapon(player, Component::ERevolver, 20, 40, 6, 6, false, "data/shot.wav");
	revolver->init();
	//revolver->deactivate();
	ComponentWeapon* machinegun = new ComponentWeapon(player, Component::EMachinegun, 10, 80, 20, 8, true, "data/shot.wav");
	machinegun->init();
	machinegun->deactivate();
	C_Target* target = new C_Target(player, "data/target.png");
	target->init();
	ComponentCollider* collider = new ComponentCollider(player, ComponentCollider::ERectCollider, ComponentCollider::EAllied, 0);
	collider->init();
	ComponentLife* life = new ComponentLife(player, 5, 0, 20);
	life->init();
	return player;
}

Entity* createBullet(vec2 pos, vec2 direction, float speed, ComponentCollider::TFaction faction) {
	Entity* bullet = new Entity();
	ComponentTransform* transform = new ComponentTransform(bullet, pos, vmake(10, 10));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(bullet, "data/bullet.png");
	renderable->init();
	ComponentInertialMove* movement = new ComponentInertialMove(bullet, direction, speed, true);
	movement->init();
	ComponentCollider* collider = new ComponentCollider(bullet, ComponentCollider::ECircleCollider, faction, -1);
	collider->init();
	ComponentLife* life = new ComponentLife(bullet, 0, 0, 0);
	life->init();
	return bullet;
}

Entity* createEnemy(int x, int y, Entity* player, int speed, int lives, int damage) {
	Entity* enemy = new Entity();
	ComponentTransform* transform = new ComponentTransform(enemy, vmake(x, y), vmake(20, 20));
	transform->init();
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", "data/player.png", 10);
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
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", "data/player.png", 10);
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
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", "data/player.png", 10);
	renderable->init();
	ComponentAIEvade* aiLong = new ComponentAIEvade(enemy, player, 4, 250);
	aiLong->init();
	ComponentAIMelee* aiMelee = new ComponentAIMelee(enemy, player, 4, 300);
	aiMelee->init();
	ComponentAIFire* aiFire = new ComponentAIFire(enemy, player);
	aiFire->init();
	ComponentWeapon* gun = new ComponentWeapon(enemy, Component::ERevolver, 40, 1, 1, 6, false);
	gun->init();
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
	ComponentRenderable* renderable = new ComponentRenderable(enemy, "data/enemy.png", "data/player.png", 10);
	renderable->init();
	ComponentAIFire* aiFire = new ComponentAIFire(enemy, dir);
	aiFire->init();
	ComponentWeapon* gun = new ComponentWeapon(enemy, Component::ERevolver, 100, 1, 1, 6, false);
	gun->init();
	ComponentCollider* collider = new ComponentCollider(enemy, ComponentCollider::ERectCollider, ComponentCollider::EEnemy, -1);
	collider->init();
	ComponentLife* life = new ComponentLife(enemy, 3, 0, 0);
	life->init();
	ComponentPoints* points = new ComponentPoints(enemy, 10);
	points->init();
	return enemy;
}

Entity* createWeaponPickup(vec2 pos, Component::TWeapon type) {
	Entity* weaponPickup = new Entity();
	ComponentTransform* transform = new ComponentTransform(weaponPickup, pos, vmake(20, 20));
	transform->init();
	std::string weaponIcon = "";
	switch (type)
	{
	case Component::ERevolver:
		weaponIcon = "R";
		break;
	case Component::EMachinegun:
		weaponIcon = "M";
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

bool checkCircleRect(const vec2& circlePos, float circleRadius, const vec2& rectPos, const vec2& rectSize) {
	vec2 closestPoint = vmake(clamp(circlePos.x, rectPos.x, rectPos.x + rectSize.x), clamp(circlePos.y, rectPos.y, rectPos.y + rectSize.y));
	return vdist(circlePos, closestPoint) < circleRadius;
}

bool checkRectRect(const vec2& rectPos1, const vec2& rectSize1, const vec2& rectPos2, const vec2& rectSize2) {
	return ((isInBounds(rectPos1.x, rectPos2.x, rectPos2.x + rectSize2.x) || isInBounds(rectPos2.x, rectPos1.x, rectPos1.x + rectSize1.x) || rectPos1.x == rectPos2.x) &&
		(isInBounds(rectPos1.y, rectPos2.y, rectPos2.y + rectSize2.y) || isInBounds(rectPos2.y, rectPos1.y, rectPos1.y + rectSize1.y) || rectPos1.y == rectPos2.y));
}