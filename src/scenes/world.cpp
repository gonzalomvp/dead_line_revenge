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
#include "../gui/string_manager.h"
#include "../gui/menu.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;

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


World::World(uint16_t level) {
	m_level       = level;
	m_player      = nullptr;
	m_isGameOver  = false;
	m_isPaused    = false;
	

	//sacar de aqui
	m_spawnData.push_back(vmake(SCR_WIDTH / 2.0f, SCR_HEIGHT));
	m_spawnData.push_back(vmake(SCR_WIDTH, SCR_HEIGHT / 2.0f));
	m_spawnData.push_back(vmake(0.0f, SCR_HEIGHT / 2.0f));
	m_spawnData.push_back(vmake(SCR_WIDTH / 2.0f, 0.0f));

	//revisar
	//m_pickupSpawnTimer = 0;
	m_enemySpawnTimer = 0;
}

World::~World() {
	CORE_UnloadPNG(texbkg);
	CORE_UnloadPNG(texlargeball);
	CORE_UnloadPNG(texsmallball);
	CORE_UnloadPNG(targetTexture);
	for (size_t i = 0; i < m_entities.size(); ++i) {
		delete m_entities[i];
	}
	g_inputManager->unregisterEvent(this, IInputManager::TEvent::EPause);
}

void World::init() {
	m_isGameOver = false;
	m_isPaused = false;
	m_score = 0;
	m_currentEnemies = 0;
	m_pickupSpawnTimer = m_pickupSpawnWait;
	g_inputManager->unregisterEvent(this, IInputManager::TEvent::EPause);
	g_inputManager->registerEvent(this, IInputManager::TEvent::EPause, 0);
	//borrar lo anterior
	for (size_t i = 0; i < m_entities.size(); ++i) {
		if (m_entities[i]) {
			delete m_entities[i];
		}
	}
	m_entities.clear();

	char *fileName = nullptr;
	switch (m_level)
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

	loadLevel(fileName);

	m_player = Entity::createPlayer(vmake(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f));
	addEntity(m_player);

	//HUD
	//createHUD(m_player);

	//addEntity(createRangeEnemy(300, 300, m_player));
	bullet.pos = vmake(-1000, -1000);
	bullet.vel = vmake(0, 0);
	bullet.pos = vmake(-1000, -1000);
	bullet.vel = vmake(0, 0);
	enemy.pos = vmake(0, 0);

	addEntity(Entity::createTurretEnemy(100, SCR_HEIGHT - 100, vmake(0, -1), m_player));
	addEntity(Entity::createTurretEnemy(SCR_WIDTH - 100, 100, vmake(-1, 0), m_player));

	//borrar
	//addEntity(createEnemy(200, 200, m_player, 0, 1, -1));
	//addEntity(createEnemy(205, 200, m_player, 0, 1, -1));
	addEntity(Entity::createWeaponPickup());
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

void World::run(float deltaTime) {
	if (!m_isPaused && !m_isGameOver) {
		// Render
		glClear(GL_COLOR_BUFFER_BIT);

		//m_player->run();

		for (size_t i = 0; i < m_entities.size(); ++i) {
			m_entities[i]->run();
		}
		checkCollisions();
		removePendingEntities();
		addPendingEntities();

		//Pick up
		if (m_pickupSpawnTimer <= m_pickupSpawnWait) {
			++m_pickupSpawnTimer;
			if (m_pickupSpawnTimer == m_pickupSpawnWait) {
				addEntity(Entity::createWeaponPickup());
			}
		}

		//enemy
		++m_enemySpawnTimer;

		if (m_enemySpawnTimer >= m_enemySpawnWait && m_currentEnemies < m_maxEnemies) {
			spawnEnemy();
			m_enemySpawnTimer = 0;
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
		Entity::TType type = (*it)->getType();
		switch (type)
		{
			case Entity::EPlayer: {
				m_isGameOver = true;
				std::string scoreMessage = g_stringManager->getText("LTEXT_GUI_SCORE_MESSAGE") + std::to_string(m_score);
				g_menuManager->getMenu(MenuManager::EGameOverMenu)->setTitle(scoreMessage.c_str());
				g_menuManager->activateMenu(MenuManager::EGameOverMenu);
				for (size_t i = 0; i < m_entities.size(); ++i) {
					delete m_entities[i];
				}
				m_entities.clear();
				m_player = nullptr;
				break;
			}
			case Entity::EPickup: {
				m_pickupSpawnTimer = 0;
				break;
			}
			case Entity::EEnemy: {
				--m_currentEnemies;
				break;
			}
			case Entity::EHUD:
				break;
		}
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
	}
	m_entitiesToRemove.clear();
}

void World::addPendingEntities() {
	for (size_t i = 0; i < m_entitiesToAdd.size(); ++i) {
		m_entities.push_back(m_entitiesToAdd[i]);
	}
	m_entitiesToAdd.clear();
}

bool checkCircleRect(const vec2& circlePos, float circleRadius, const vec2& rectPos, const vec2& rectSize) {
	vec2 closestPoint = vmake(clamp(circlePos.x, rectPos.x, rectPos.x + rectSize.x), clamp(circlePos.y, rectPos.y, rectPos.y + rectSize.y));
	return vdist(circlePos, closestPoint) < circleRadius;
}

bool checkRectRect(const vec2& rectPos1, const vec2& rectSize1, const vec2& rectPos2, const vec2& rectSize2) {
	return ((isInBounds(rectPos1.x, rectPos2.x, rectPos2.x + rectSize2.x) || isInBounds(rectPos2.x, rectPos1.x, rectPos1.x + rectSize1.x) || rectPos1.x == rectPos2.x) &&
		(isInBounds(rectPos1.y, rectPos2.y, rectPos2.y + rectSize2.y) || isInBounds(rectPos2.y, rectPos1.y, rectPos1.y + rectSize1.y) || rectPos1.y == rectPos2.y));
}

bool World::onEvent(const IInputManager::Event& event) {
	IInputManager::TEvent eventType = event.getType();
	if (eventType == IInputManager::TEvent::EPause) {
		m_isPaused = !m_isPaused;
		//ShowCursor(m_isPaused);
		if (m_isPaused) {
			m_player->deactivate();
			g_menuManager->activateMenu(MenuManager::EPauseMenu);
		}
		else {
			m_player->activate();
			g_menuManager->deactivateMenu();
		}
	}
	return true;
}

bool World::loadLevel(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (!file) return false;

	fseek(file, 0, SEEK_END);
	std::vector<char> fileData(ftell(file));
	fseek(file, 0, SEEK_SET);
	FileReadStream is(file, fileData.data(), fileData.size());
	Document doc;
	doc.ParseStream(is);

	//leer de fichero
	m_pickupSpawnWait = 300;
	m_pickupSpawnTimer = m_pickupSpawnWait;
	float totalProbability = 0.0f;
	int spawnPoints = doc["spawnPoints"].GetInt();
	int spawnFrecuency = doc["spawnFrecuency"].GetInt();
	int concurrentEnemies = doc["concurrentEnemies"].GetInt();

	TEnemyData meleeEnemy;
	meleeEnemy.type = EMelee;
	meleeEnemy.speed = doc["meleeEnemy"].GetObject()["speed"].GetInt();
	meleeEnemy.life = doc["meleeEnemy"].GetObject()["life"].GetInt();
	meleeEnemy.damage = doc["meleeEnemy"].GetObject()["damage"].GetInt();
	totalProbability += doc["meleeEnemy"].GetObject()["spawnProbability"].GetFloat();
	meleeEnemy.spawnProbability = totalProbability;

	TEnemyData bigEnemy;
	bigEnemy.type = EBig;
	bigEnemy.speed = doc["bigEnemy"].GetObject()["speed"].GetInt();
	bigEnemy.life = doc["bigEnemy"].GetObject()["life"].GetInt();
	bigEnemy.damage = doc["bigEnemy"].GetObject()["damage"].GetInt();
	totalProbability += doc["bigEnemy"].GetObject()["spawnProbability"].GetFloat();
	bigEnemy.spawnProbability = totalProbability;

	TEnemyData rangeEnemy;
	rangeEnemy.type = ERange;
	rangeEnemy.speed = doc["rangeEnemy"].GetObject()["speed"].GetInt();
	rangeEnemy.life = doc["rangeEnemy"].GetObject()["life"].GetInt();
	rangeEnemy.damage = doc["rangeEnemy"].GetObject()["damage"].GetInt();
	totalProbability += doc["rangeEnemy"].GetObject()["spawnProbability"].GetFloat();
	rangeEnemy.spawnProbability = totalProbability;

	m_spawnPoints = spawnPoints;
	m_enemySpawnWait = spawnFrecuency;
	m_maxEnemies = concurrentEnemies;

	m_enemyData.push_back(meleeEnemy);
	m_enemyData.push_back(bigEnemy);
	m_enemyData.push_back(rangeEnemy);

	fclose(file);
	return true;
}

void World::spawnEnemy() {
	vec2 spawnLocation = m_spawnData[rand() % m_spawnPoints];
	float enemyType = CORE_FRand(0.0f, 1.0f);
	Entity* enemy = nullptr;
	for (size_t i = 0; i < m_enemyData.size(); i++)
	{
		if (enemyType <= m_enemyData[i].spawnProbability) {
			switch (m_enemyData[i].type) {
				case EMelee:
					enemy = Entity::createEnemy(spawnLocation.x, spawnLocation.y, g_world->getPlayer(), m_enemyData[i].speed, m_enemyData[i].life, m_enemyData[i].damage);
					break;
				case EBig:
					enemy = Entity::createBigEnemy(spawnLocation.x, spawnLocation.y, g_world->getPlayer(), m_enemyData[i].speed, m_enemyData[i].life, m_enemyData[i].damage);
					break;
				case ERange:
					enemy = Entity::createRangeEnemy(spawnLocation.x, spawnLocation.y, g_world->getPlayer());
					break;
			}
			break;
		}
	}

	g_world->addEntity(enemy);
	++m_currentEnemies;
}