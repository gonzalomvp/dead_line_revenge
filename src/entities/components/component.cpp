#include "../../common/stdafx.h"
#include "component.h"
#include "../entity.h"
#include "../message.h"
#include "../../scenes/world.h"
#include "../../gui/string_manager.h"
#include "../../engine/graphics_engine.h"

#include <algorithm>

#define EVASION_ANGLE         45.0f
#define EVASION_MIN_WALL_DIST 20.0f

//=============================================================================
// Component class
//=============================================================================
void Component::init() {
	m_owner->addComponent(this);
	if (m_activationDelay == 0) {
		activate();
	}
}

void Component::destroy() {
	deactivate();
	m_owner->removeComponent(this);
}

void Component::activate() {
	m_isActive = true;
}

void Component::deactivate() {
	m_isActive = false;
}

void Component::run(float deltaTime) {
	if (m_activationTimer < m_activationDelay) {
		++m_activationTimer;
		if (m_activationTimer == m_activationDelay) {
			activate();
		}
	}
}

//=============================================================================
// ComponentTransform class
//=============================================================================
void ComponentTransform::run(float deltaTime) {
	Component::run(deltaTime);
	m_size = vadd(m_size, m_sizeIncrement);
}

void ComponentTransform::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageSetTransform *msgSetTransform = dynamic_cast<MessageSetTransform*>(message);
	if (msgSetTransform) {
		m_pos = msgSetTransform->pos;
		m_size = msgSetTransform->size;
		bool outOfBounds = false;
		vec2 bounceDirection;

		if (m_pos.x > SCR_WIDTH - m_size.x * 0.5) {
			m_pos.x = WORLD_WIDTH - m_size.x * 0.5;
			outOfBounds = true;
			bounceDirection = vmake(-1.0f , 1.0f);
		}
		else if (m_pos.x < 0 + m_size.x * 0.5) {
			m_pos.x = 0 + m_size.x * 0.5;
			outOfBounds = true;
			bounceDirection = vmake(-1.0f, 1.0f);
		}
		if (m_pos.y > WORLD_HEIGHT - m_size.y * 0.5) {
			m_pos.y = WORLD_HEIGHT - m_size.y * 0.5;
			outOfBounds = true;
			bounceDirection = vmake(1.0f, -1.0f);
		}
		else if (m_pos.y < 0 + m_size.y * 0.5) {
			m_pos.y = 0 + m_size.y * 0.5;
			outOfBounds = true;
			bounceDirection = vmake(1.0f, -1.0f);
		}
		MessageTransformChanged msgTransformChanged;
		msgTransformChanged.pos = m_pos;
		msgTransformChanged.size = m_size;
		m_owner->receiveMessage(&msgTransformChanged);

		if (outOfBounds) {
			MessageCheckCollision msgCheckCollision;
			msgCheckCollision.overlap = true;
			msgCheckCollision.deltaLife = -1;
			msgCheckCollision.collisionChannel = ComponentCollider::EBoundaries;
			msgCheckCollision.bounceDirection = bounceDirection;
			m_owner->receiveMessage(&msgCheckCollision);
		}
	}
	else {
		MessageGetTransform *msgGetTransform = dynamic_cast<MessageGetTransform*>(message);
		if (msgGetTransform) {
			msgGetTransform->pos = m_pos;
			msgGetTransform->size = m_size;
		}
	}
}

//=============================================================================
// ComponentLife class
//=============================================================================
ComponentLife::ComponentLife(Entity* owner, int life, int timeToLive, int invencibleTime) : Component(owner), m_life(life), m_timeToLive(timeToLive), m_invencibleTime(invencibleTime) {
	m_lifeTimer = 0;
	m_invencibleTimer = 0;
}

void ComponentLife::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	if (m_invencibleTimer < m_invencibleTime) {
		++m_invencibleTimer;
	}

	if (m_lifeTimer < m_timeToLive) {
		++m_lifeTimer;
		if (m_lifeTimer == m_timeToLive) {
			m_life = 0;
		}
	}
	if (m_life == 0) {
		MessageDestroy msgDestroy;
		m_owner->receiveMessage(&msgDestroy);
		m_owner->deactivate();
		g_world->removeEntity(m_owner);
	}
}

void ComponentLife::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageGetLife *msgGetLife = dynamic_cast<MessageGetLife*>(message);
	if (msgGetLife) {
		msgGetLife->currentLife = m_life;
		
	}
	else {
		MessageChangeLife *msgChangeLife = dynamic_cast<MessageChangeLife*>(message);
		if (msgChangeLife && (m_life != -1) && (m_invencibleTimer >= m_invencibleTime)) {
			m_life += msgChangeLife->deltaLife;
			m_invencibleTimer = 0;
			if (m_life <= 0) {
				m_life = 0;
			}
		}
		else {
			MessageDestroy *msgdestroy = dynamic_cast<MessageDestroy*>(message);
			if (msgdestroy) {
				m_life = 0;
			}
		}
	}
}

//=============================================================================
// ComponentMove class
//=============================================================================
void ComponentMove::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	if (vlen2(m_direction) != 0) {
		MessageGetTransform msgGetTransform;
		m_owner->receiveMessage(&msgGetTransform);

		MessageSetTransform msgSetTransform;
		msgSetTransform.pos = vadd(msgGetTransform.pos, vscale(vnorm(m_direction), m_speed));
		msgSetTransform.size = msgGetTransform.size;
		m_owner->receiveMessage(&msgSetTransform);
	}
	
	if (!m_hasInertia) {
		m_direction = vmake(0.0f, 0.0f);
	}
}

void ComponentMove::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageAddMovement *msgAddMovement = dynamic_cast<MessageAddMovement*>(message);
	if (msgAddMovement) {
		m_direction = vadd(m_direction, msgAddMovement->dir);
	}
	else {
		MessageCheckCollision *msgCheckCollision = dynamic_cast<MessageCheckCollision*>(message);
		if (msgCheckCollision && msgCheckCollision->overlap && m_hasBounce && vlen2(msgCheckCollision->bounceDirection) > 0) {
			m_direction = vmake(m_direction.x * msgCheckCollision->bounceDirection.x, m_direction.y * msgCheckCollision->bounceDirection.y);
		}
	}
}

//=============================================================================
// ComponentRenderable class
//=============================================================================
ComponentRenderable::ComponentRenderable(Entity* owner, const char* texture, float angle, float alpha, int priority, int hitTime) : Component(owner), m_texture(texture), m_angle(angle), m_alpha(alpha), m_priority(priority), m_hitTime(hitTime) {
	m_hitTimer = m_hitTime;
}

ComponentRenderable::~ComponentRenderable() {
	g_graphicsEngine->removeGfxEntity(m_sprite);
}

void ComponentRenderable::init() {
	Component::init();
	m_sprite = new Sprite(g_graphicsEngine->getTexture(m_texture), m_priority, m_alpha, m_angle);
	g_graphicsEngine->addGfxEntity(m_sprite);
}

void ComponentRenderable::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	MessageGetTransform msgGetTransform;
	m_owner->receiveMessage(&msgGetTransform);
	m_sprite->setPos(msgGetTransform.pos);
	m_sprite->setSize(msgGetTransform.size);

	if (m_hitTimer < m_hitTime) {
		++m_hitTimer;
		int hitAnim = m_hitTimer % 2;
		if (hitAnim) {
			m_sprite->deactivate();
		}
		else {
			m_sprite->activate();
		}
		if (m_hitTimer == m_hitTime) {
			//m_sprite->setTexture(g_graphicsEngine->getTexture(m_texture));
			m_sprite->activate();
		}
	}
}

void ComponentRenderable::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageTransformChanged *msgTransformChanged = dynamic_cast<MessageTransformChanged*>(message);
	if (msgTransformChanged) {
		m_sprite->setPos(msgTransformChanged->pos);
		m_sprite->setSize(msgTransformChanged->size);
	}
	else {
		MessageChangeLife *msgChangeLife = dynamic_cast<MessageChangeLife*>(message);
		if (msgChangeLife && msgChangeLife->deltaLife < 0) {
			m_hitTimer = 0;
		}
		else {
			MessageAimDirection* messageAimDirection = dynamic_cast<MessageAimDirection*>(message);
			if (messageAimDirection) {
				m_sprite->setAngle(vangle(messageAimDirection->direction));
			}
		}
	}
}

//=============================================================================
// ComponentPlayerController class
//=============================================================================
void ComponentPlayerController::init() {
	Component::init();
	g_inputManager->registerEvent(this, IInputManager::TEvent::EKey, 0);
	g_inputManager->registerEvent(this, IInputManager::TEvent::EMouse, 0);
}

ComponentPlayerController::~ComponentPlayerController() {
	g_inputManager->unregisterEvent(this, IInputManager::TEvent::EKey);
	g_inputManager->unregisterEvent(this, IInputManager::TEvent::EMouse);
}

bool ComponentPlayerController::onEvent(const IInputManager::Event& event) {
	if (!m_isActive)
		return false;
	//Revisar inputmanager
	IInputManager::TEvent eventType = event.getType();
	if (eventType == IInputManager::TEvent::EKey) {
		const IInputManager::KeyEvent keyEvent = *static_cast<const IInputManager::KeyEvent*>(&event);
		vec2 direction = vmake(0, 0);
		switch (keyEvent.key) {
			case 'A':
				direction = vmake(-1, 0);
				break;
			case 'D':
				direction = vmake(1, 0);
				break;
			case 'W':
				direction = vmake(0, 1);
				break;
			case 'S':
				direction = vmake(0, -1);
				break;
			case VK_SPACE:
				MessageReload msgReload;
				m_owner->receiveMessage(&msgReload);
				break;
		}
		if (vlen2(direction) != 0) {
			/*MessageGetTransform msgGetTransform;
			m_owner->receiveMessage(&msgGetTransform);

			MessageSetTransform msgSetTransform;
			msgSetTransform.pos = vadd(msgGetTransform.pos, vscale(vnorm(direction), m_speed));
			msgSetTransform.size = msgGetTransform.size;
			m_owner->receiveMessage(&msgSetTransform);*/

			MessageAddMovement msgAddMovement;
			msgAddMovement.dir = direction;
			m_owner->receiveMessage(&msgAddMovement);
		}
	}

	if (eventType == IInputManager::TEvent::EMouse) {
		const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);
		if (mouseEvent.mouseButton == mouseEvent.BLeft) {
			MessageFire messageFire;
			if (mouseEvent.mouseButtonAction == mouseEvent.AButtonDown) {
				messageFire.isFiring = true;
			}
			else if (mouseEvent.mouseButtonAction == mouseEvent.AButtonUp) {
				messageFire.isFiring = false;
			}
			m_owner->receiveMessage(&messageFire);
		}
		else if (mouseEvent.mouseButton == mouseEvent.BRight && mouseEvent.mouseButtonAction == mouseEvent.AButtonDown)
		{
			MessageReload msgReload;
			m_owner->receiveMessage(&msgReload);
		}
		/*const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);
		setTargetPos(vmake(mouseEvent.x, mouseEvent.y));
		if (mouseEvent.buttonMask == 1) {
		fire();
		}
		if (mouseEvent.buttonMask == 2) {
		dash();
		}*/
	}
	return true;
}

//=============================================================================
// ComponentWeapon class
//=============================================================================
ComponentWeapon::ComponentWeapon(Entity* owner, TWeaponData weaponData) : Component(owner), m_weaponData(weaponData) {
	m_remoteBullet   = nullptr;
	m_aimDirection   = vmake(0.0f, 0.0f);
	m_isFiring       = false;
	m_currentBullets = m_weaponData.capacity;
	m_fireTimer      = m_weaponData.fireRate;
	m_reloadTimer    = m_weaponData.reloadTime;
}

void ComponentWeapon::init() {
	Component::init();
}

void ComponentWeapon::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	if (m_fireTimer < m_weaponData.fireRate) {
		++m_fireTimer;
	}
	if (m_reloadTimer <= m_weaponData.reloadTime) {
		++m_reloadTimer;
		if (m_reloadTimer == m_weaponData.reloadTime) {
			m_currentBullets = m_weaponData.capacity;
		}
	}

	if (m_isFiring && m_remoteBullet) {
		MessageDestroy msgDestroy;
		m_remoteBullet->receiveMessage(&msgDestroy);
		m_remoteBullet = nullptr;
		m_isFiring = false;
		m_currentBullets = m_weaponData.capacity;
	}

	else if (m_isFiring && m_fireTimer >= m_weaponData.fireRate && m_reloadTimer >= m_weaponData.reloadTime && m_currentBullets > 0) {
		m_fireTimer = 0;
		--m_currentBullets;
		if (m_currentBullets == 0) {
			m_reloadTimer = 0;
		}
		if (!m_weaponData.isAutomatic) {
			m_isFiring = false;
		}
		
		MessageGetTransform messageGetTranform;
		m_owner->receiveMessage(&messageGetTranform);
		//MessageGetCollider msgGetCollider;
		//m_owner->receiveMessage(&msgGetCollider);

		switch (m_weaponData.type)
		{
			case EShotgun: {
				vec2 bulletDir = m_aimDirection;
				g_world->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), bulletDir, m_weaponData.bulletSpeed, m_weaponData.bulletDamage, m_weaponData.bulletLife, m_weaponData.bulletRange, m_weaponData.isExplossive, m_weaponData.isBouncy, m_owner->getType(), "data/shotgunBullet.png");
				float angle = vangle(m_aimDirection);
				angle += SHOTGUN_DISP_ANGLE;
				bulletDir = vunit(DEG2RAD(angle));
				g_world->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), bulletDir, m_weaponData.bulletSpeed, m_weaponData.bulletDamage, m_weaponData.bulletLife, m_weaponData.bulletRange, m_weaponData.isExplossive, m_weaponData.isBouncy, m_owner->getType(), "data/shotgunBullet.png");
				angle = vangle(m_aimDirection);
				angle -= SHOTGUN_DISP_ANGLE;
				bulletDir = vunit(DEG2RAD(angle));
				g_world->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), bulletDir, m_weaponData.bulletSpeed, m_weaponData.bulletDamage, m_weaponData.bulletLife, m_weaponData.bulletRange, m_weaponData.isExplossive, m_weaponData.isBouncy, m_owner->getType(), "data/shotgunBullet.png");
				CORE_PlayMusic(CORE_LoadWav("data/shotgun.wav"));
				break;
			}
			case EMines: {
				g_world->createMine(messageGetTranform.pos);
				CORE_PlayMusic(CORE_LoadWav("data/mine.wav"));
				break;
			}
			case EC4: {
				m_remoteBullet = g_world->createBullet(messageGetTranform.pos, vmake(20.0f, 20.0f), m_aimDirection, m_weaponData.bulletSpeed, m_weaponData.bulletDamage, m_weaponData.bulletLife, m_weaponData.bulletRange, m_weaponData.isExplossive, m_weaponData.isBouncy, m_owner->getType(), "data/c4.png");
				CORE_PlayMusic(CORE_LoadWav("data/mine.wav"));
				break;
			}
			case ERocketLauncher: {
				g_world->createBullet(messageGetTranform.pos, vmake(15.0f, 15.0f), m_aimDirection, m_weaponData.bulletSpeed, m_weaponData.bulletDamage, m_weaponData.bulletLife, m_weaponData.bulletRange, m_weaponData.isExplossive, m_weaponData.isBouncy, m_owner->getType(), "data/rocket.png");
				CORE_PlayMusic(CORE_LoadWav("data/rocketlauncher.wav"));
				break;
			}
			case ENuclearBomb: {
				g_world->createNuclearBomb();
				break;
			}
			default: {
				g_world->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), m_aimDirection, m_weaponData.bulletSpeed, m_weaponData.bulletDamage, m_weaponData.bulletLife, m_weaponData.bulletRange, m_weaponData.isExplossive, m_weaponData.isBouncy, m_owner->getType(), "data/bullet.png");
				CORE_PlayMusic(CORE_LoadWav("data/shot.wav"));
				break;
			}
		}

		MessageFireDone messageFireDone;
		m_owner->receiveMessage(&messageFireDone);
	}
}

void ComponentWeapon::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageWeaponChange *msgWeaponChange = dynamic_cast<MessageWeaponChange*>(message);
	if (msgWeaponChange) {
		m_weaponData = g_world->getWeaponData(msgWeaponChange->weapon);
		m_isFiring = false;
		m_currentBullets = m_weaponData.capacity;
		m_fireTimer = m_weaponData.fireRate;
		m_reloadTimer = m_weaponData.reloadTime;
	}
	else {
		MessageFire *msgFire = dynamic_cast<MessageFire*>(message);
		if (msgFire) {
			m_isFiring = msgFire->isFiring;
		}
		else {
			MessageAimDirection *msgAimDirection = dynamic_cast<MessageAimDirection*>(message);
			if (msgAimDirection) {
				m_aimDirection = msgAimDirection->direction;
			}
			else {
				MessageAmmoInfo *msgAmmoInfo = dynamic_cast<MessageAmmoInfo*>(message);
				if (msgAmmoInfo) {
					msgAmmoInfo->currentAmmo = m_currentBullets;
					msgAmmoInfo->totalAmmo = m_weaponData.capacity;
					msgAmmoInfo->reloadPercent = m_reloadTimer * 1.0f / m_weaponData.reloadTime;
					if (msgAmmoInfo->reloadPercent > 1.0f) {
						msgAmmoInfo->reloadPercent = 1.0f;
					}
				}
				else {
					MessageReload *msgReload = dynamic_cast<MessageReload*>(message);
					if (msgReload && m_currentBullets < m_weaponData.capacity && m_reloadTimer >= m_weaponData.reloadTime) {
						m_isFiring = false;
						m_reloadTimer = 0;
					}
				}
			}
		}
	}
}

//=============================================================================
// ComponentExplossive class
//=============================================================================
void ComponentExplossive::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageDestroy *msgDestroy = dynamic_cast<MessageDestroy*>(message);
	if (msgDestroy) {
		MessageGetTransform messageSelfPos;
		m_owner->receiveMessage(&messageSelfPos);
		g_world->createExplossion(messageSelfPos.pos, vmake(100, 100));
	}
}

//=============================================================================
// ComponentAIMelee class
//=============================================================================
ComponentAIMelee::ComponentAIMelee(Entity* owner, Entity* player, float speed, float maxDistance) : Component(owner), m_player(player), m_speed(speed), m_maxDistance(maxDistance) {
	m_offset = vmake(CORE_FRand(-20, 20), CORE_FRand(-20, 20));
}
void ComponentAIMelee::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	MessageGetTransform messageSelfPos;
	m_owner->receiveMessage(&messageSelfPos);
	MessageGetTransform messagePlayerPos;
	m_player->receiveMessage(&messagePlayerPos);
	vec2 direction = vsub(vadd(messagePlayerPos.pos, m_offset), messageSelfPos.pos);

	if (vlen(direction) > m_maxDistance) {
		MessageSetTransform msgSetTransform;
		msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(direction), m_speed));
		msgSetTransform.size = messageSelfPos.size;
		m_owner->receiveMessage(&msgSetTransform);
	}
}

//=============================================================================
// ComponentAIEvade class
//=============================================================================
void ComponentAIEvade::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	MessageGetTransform messageSelfPos;
	m_owner->receiveMessage(&messageSelfPos);
	MessageGetTransform messagePlayerPos;
	m_player->receiveMessage(&messagePlayerPos);

	//Move in opposite direction
	vec2 escapeDirection = vsub(messageSelfPos.pos, messagePlayerPos.pos);
	float playerDistance = vlen(escapeDirection);

	//Keep min distance with player
	if (playerDistance < m_range) {
		vec2 moveDir = vmake(0, 0);
		float angle1 = vangle(escapeDirection);
		float angle2 = angle1;

		//Find the best direction for evasion using different angles
		while (vlen2(moveDir) == 0) {
			angle1 += EVASION_ANGLE;
			angle2 -= EVASION_ANGLE;

			//Calculate the distance to the screen limits with both directions
			vec2 intersectiontWithWall1 = calculatIntersectionWithWall(messageSelfPos.pos, angle1);
			float distToWall1 = vlen(vsub(intersectiontWithWall1, messageSelfPos.pos));
			vec2 intersectiontWithWall2 = calculatIntersectionWithWall(messageSelfPos.pos, angle2);
			float distToWall2 = vlen(vsub(intersectiontWithWall2, messageSelfPos.pos));

			//Pick a direction if it is not too close to the screen limits
			if (distToWall1 > distToWall2 && distToWall1 > EVASION_MIN_WALL_DIST) {
				moveDir = vsub(intersectiontWithWall1, messageSelfPos.pos);
			}
			else if (distToWall2 > distToWall1 && distToWall2 > EVASION_MIN_WALL_DIST) {
				moveDir = vsub(intersectiontWithWall2, messageSelfPos.pos);
			}
		}

		MessageSetTransform msgSetTransform;
		msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(moveDir), m_speed));
		msgSetTransform.size = messageSelfPos.size;
		m_owner->receiveMessage(&msgSetTransform);
	}
}

vec2 ComponentAIEvade::calculatIntersectionWithWall(vec2 position, float angle) {
	// Ver si se puede hacer simplemente analizando las coordenadas x, y de la posicion y las posiciones limites de las lineas WORLD_HEIGHT, WORLD_WIDTH, 0 ,0
	// por ejemplo (WORLD_HEIGHT / componente y del movimiento) = cercania
	vec2 moveDir = vunit(DEG2RAD(angle));
	vec2 intersection1;
	vec2 intersection2;

	if (moveDir.y > 0)
		LineLineIntersect(position, vadd(position, moveDir), vmake(0, WORLD_HEIGHT), vmake(WORLD_WIDTH, WORLD_HEIGHT), intersection1);
	else
		LineLineIntersect(position, vadd(position, moveDir), vmake(0, 0), vmake(WORLD_WIDTH, 0), intersection1);

	if (moveDir.x > 0)
		LineLineIntersect(position, vadd(position, moveDir), vmake(WORLD_WIDTH, 0), vmake(WORLD_WIDTH, WORLD_HEIGHT), intersection2);
	else
		LineLineIntersect(position, vadd(position, moveDir), vmake(0, 0), vmake(0, WORLD_HEIGHT), intersection2);

	float distUpDownMargin = vlen(vsub(intersection1, position));
	float distLeftRight = vlen(vsub(intersection2, position));

	if (distUpDownMargin < distLeftRight)
		return intersection1;
	else
		return intersection2;
}

//=============================================================================
// ComponentAIFire class
//=============================================================================
void ComponentAIFire::init() {
	Component::init();
	if (m_fireDirections.size() > 0 && m_shuffle) {
		std::random_shuffle(m_fireDirections.begin(), m_fireDirections.end());
	}
}

void ComponentAIFire::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	MessageFire msgFire;
	msgFire.isFiring = true;
	m_owner->receiveMessage(&msgFire);

	if (m_player) {
		MessageGetTransform messageSelfPos;
		m_owner->receiveMessage(&messageSelfPos);
		MessageGetTransform messagePlayerPos;
		m_player->receiveMessage(&messagePlayerPos);
		MessageAimDirection messageAimDirection;
		messageAimDirection.direction = vnorm(vsub(messagePlayerPos.pos, messageSelfPos.pos));
		m_owner->receiveMessage(&messageAimDirection);
	}
	else {
		MessageAimDirection messageAimDirection;
		messageAimDirection.direction = m_fireDirections[m_currentFireDirection];
		m_owner->receiveMessage(&messageAimDirection);
	}
}

void ComponentAIFire::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageFireDone *msgFireDone = dynamic_cast<MessageFireDone*>(message);
	if (msgFireDone && ! m_player) {
		++m_currentFireDirection;
		if (m_currentFireDirection >= m_fireDirections.size()) {
			m_currentFireDirection = 0;
		}
		MessageAimDirection messageAimDirection;
		messageAimDirection.direction = m_fireDirections[m_currentFireDirection];
		m_owner->receiveMessage(&messageAimDirection);
	}
}

//=============================================================================
// ComponentCollider class
//=============================================================================
void ComponentCollider::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	MessageGetTransform message;
	m_owner->receiveMessage(&message);
	m_center = message.pos;
	m_size   = message.size;
}

void ComponentCollider::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageCheckCollision *msgCheckCollision = dynamic_cast<MessageCheckCollision*>(message);
	if (msgCheckCollision) {
		bool overlap = false;
		Entity* other = msgCheckCollision->other;
		if (msgCheckCollision->other) {
			msgCheckCollision->type = m_type;
			msgCheckCollision->center = m_center;
			msgCheckCollision->size = m_size;
			msgCheckCollision->deltaLife = m_deltaLife;
			msgCheckCollision->collisionChannel = m_collisionChannel;
			msgCheckCollision->collisionChannelsResponse = m_collisionChannelsResponse;
			msgCheckCollision->other = nullptr;
			other->receiveMessage(msgCheckCollision);
			overlap = msgCheckCollision->overlap;
		}
		else if ((m_collisionChannelsResponse & msgCheckCollision->collisionChannel) || (msgCheckCollision->collisionChannelsResponse & m_collisionChannel)) {
			overlap = msgCheckCollision->overlap;
			if (!overlap) {
				switch (m_type) {
				case ComponentCollider::ECircleCollider:
					switch (msgCheckCollision->type) {
					case ComponentCollider::ECircleCollider:
						overlap = checkCircleCircle(m_center, m_size.x * 0.5f, msgCheckCollision->center, msgCheckCollision->size.x * 0.5f);
						break;
					case ComponentCollider::ERectCollider:
						overlap = checkCircleRect(m_center, m_size.x * 0.5f, vsub(msgCheckCollision->center, vscale(msgCheckCollision->size, 0.5f)), msgCheckCollision->size);
						break;
					}
					break;
				case ComponentCollider::ERectCollider:
					switch (msgCheckCollision->type) {
					case ComponentCollider::ECircleCollider:
						overlap = checkCircleRect(msgCheckCollision->center, msgCheckCollision->size.x * 0.5f, vsub(m_center, vscale(m_size, 0.5f)), m_size);
						break;
					case ComponentCollider::ERectCollider:
						overlap = checkRectRect(vsub(m_center, vscale(m_size, 0.5f)), m_size, vsub(msgCheckCollision->center, vscale(msgCheckCollision->size, 0.5f)), msgCheckCollision->size);
						break;
					}
					break;
				}
				if (overlap) {
					overlap = false;
					if (m_collisionChannelsResponse & msgCheckCollision->collisionChannel) {
						overlap = true;
					}
					if (msgCheckCollision->collisionChannelsResponse & m_collisionChannel) {
						msgCheckCollision->overlap = true;
						msgCheckCollision->deltaLife = m_deltaLife;
					}
				}
			}
		}
		if (overlap) {
			MessageChangeLife mgsChangeLife;
			mgsChangeLife.deltaLife = msgCheckCollision->deltaLife;
			m_owner->receiveMessage(&mgsChangeLife);
		}
	}
	
	//MessageGetCollider *msgCollider = dynamic_cast<MessageGetCollider*>(message);
	//if (msgCollider) {
	//	msgCollider->type      = m_type;
	//	msgCollider->faction   = m_faction;
	//	msgCollider->center    = m_center;
	//	msgCollider->size      = m_size;
	//	msgCollider->deltaLife = m_deltaLife;
	//	msgCollider->collisionChannel = m_collisionChannel;
	//}
	//else {
	//	MessageCollision *msgCollision = dynamic_cast<MessageCollision*>(message);
	//	if (msgCollision && m_faction != ENeutral && msgCollision->faction != ENeutral) {
	//		MessageChangeLife mgsChangeLife;
	//		mgsChangeLife.deltaLife = msgCollision->deltaLife;
	//		m_owner->receiveMessage(&mgsChangeLife);
	//	}
	//}
}

//=============================================================================
// ComponentPoints class
//=============================================================================
void ComponentPoints::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageDestroy *msgDestroy = dynamic_cast<MessageDestroy*>(message);
	if (msgDestroy) {
		g_world->addPoints(m_points);
	}
}

//Revisar el tema colisiones y facciones
//=============================================================================
// ComponentWeaponPickup class
//=============================================================================
void ComponentWeaponPickup::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageDestroy *msgDestroy = dynamic_cast<MessageDestroy*>(message);
	if (msgDestroy) {
		MessageWeaponChange msgWeapon;
		msgWeapon.weapon = m_weapon;
		g_world->getPlayer()->receiveMessage(&msgWeapon);

		std::string hudMessage = g_stringManager->getText("LTEXT_GUI_PICKUP_MESSAGE");
		switch (m_weapon)
		{
			case ERevolver:
				hudMessage += g_stringManager->getText("LTEXT_GUI_REVOLVER_MESSAGE");
				break;
			case EMachinegun:
				hudMessage += g_stringManager->getText("LTEXT_GUI_MACHINEGUN_MESSAGE");
				break;
			case EShotgun:
				hudMessage += g_stringManager->getText("LTEXT_GUI_SHOTGUN_MESSAGE");
				break;
			case EMines:
				hudMessage += g_stringManager->getText("LTEXT_GUI_MINES_MESSAGE");
				break;
			case EC4:
				hudMessage += g_stringManager->getText("LTEXT_GUI_C4_MESSAGE");
				break;
			case ERocketLauncher:
				hudMessage += g_stringManager->getText("LTEXT_GUI_ROCKETLAUNCHER_MESSAGE");
				break;
			case ENuclearBomb:
				hudMessage += g_stringManager->getText("LTEXT_GUI_NUCLEAR_MESSAGE");
				break;
		}

		g_world->createHUDMessage(hudMessage, vmake((WORLD_WIDTH / 2) - (hudMessage.length() / 2.0f * 16), 20), 100);
	}
}

//=============================================================================
// ComponentHUDMessage class
//=============================================================================
ComponentHUDMessage::~ComponentHUDMessage() {
	g_graphicsEngine->removeGfxEntity(m_message);
}

void ComponentHUDMessage::init() {
	Component::init();
	m_message = new Text(m_messageText, 1, m_pos);
	g_graphicsEngine->addGfxEntity(m_message);
}

//=============================================================================
// ComponentHUD class
//=============================================================================
ComponentHUD::~ComponentHUD() {
	g_graphicsEngine->removeGfxEntity(m_life);
	g_graphicsEngine->removeGfxEntity(m_score);
	g_graphicsEngine->removeGfxEntity(m_ammo);
	g_graphicsEngine->removeGfxEntity(m_fps);
	g_graphicsEngine->removeGfxEntity(m_target);
	g_graphicsEngine->removeGfxEntity(m_reloadAnim);

	g_inputManager->unregisterEvent(this, IInputManager::TEvent::EMouse);
}

void ComponentHUD::init() {
	Component::init();
	m_life = new Text("", 1, vmake(20, 430));
	g_graphicsEngine->addGfxEntity(m_life);

	g_graphicsEngine->addGfxEntity(new Text("LIFE", 1, vmake(20, 450)));
	g_graphicsEngine->addGfxEntity(new Text("SCORE", 1, vmake(110, 450)));
	g_graphicsEngine->addGfxEntity(new Text("AMMO", 1, vmake(210, 450)));

	m_score = new Text("| 120", 1, vmake(110, 430));
	g_graphicsEngine->addGfxEntity(m_score);

	m_ammo = new Text("| 6/-", 1, vmake(210, 430));
	g_graphicsEngine->addGfxEntity(m_ammo);

	m_fps = new Text("", 1, vmake(300, 300));
	g_graphicsEngine->addGfxEntity(m_fps);

	m_target = new Sprite(g_graphicsEngine->getTexture("data/target.png"), 1);
	m_target->setSize(vmake(36, 36));
	g_graphicsEngine->addGfxEntity(m_target);

	m_reloadAnim = new Sprite(g_graphicsEngine->getTexture("data/energy-bar-fill.png"), 1);
	g_graphicsEngine->addGfxEntity(m_reloadAnim);

	g_inputManager->registerEvent(this, IInputManager::TEvent::EMouse, 0);
}

void ComponentHUD::run(float deltaTime) {
	Component::run(deltaTime);
	MessageGetLife msgLife;
	m_owner->receiveMessage(&msgLife);
	m_life->setText(std::to_string(msgLife.currentLife));
	
	m_score->setText(std::to_string(g_world->getScore()));

	MessageAmmoInfo msgAmmo;
	m_owner->receiveMessage(&msgAmmo);
	m_ammo->setText(std::to_string(msgAmmo.currentAmmo) + "/" + std::to_string(msgAmmo.totalAmmo));

	MessageGetTransform msgTransform;
	m_owner->receiveMessage(&msgTransform);

	// Full sprite reload animation
	//m_reloadAnim->setPos(vmake(msgTransform.pos.x, msgTransform.pos.y - msgTransform.size.y * msgAmmo.reloadPercent * 0.5f));
	//m_reloadAnim->setSize(vmake(msgTransform.size.x, msgTransform.size.y * (1 - msgAmmo.reloadPercent)));

	// Right sprite reload animation
	m_reloadAnim->setPos(vmake(msgTransform.pos.x + msgTransform.size.x * 0.5f + 5, msgTransform.pos.y - msgTransform.size.y * msgAmmo.reloadPercent * 0.5f));
	m_reloadAnim->setSize(vmake(5, msgTransform.size.y * (1 - msgAmmo.reloadPercent)));

	// Top sprite reload animation
	//m_reloadAnim->setPos(vmake(msgTransform.pos.x - msgTransform.size.x * msgAmmo.reloadPercent * 0.5f, msgTransform.pos.y + msgTransform.size.y * 0.5f + 5));
	//m_reloadAnim->setSize(vmake(msgTransform.size.x * (1 - msgAmmo.reloadPercent), 5));
}

bool ComponentHUD::onEvent(const IInputManager::Event& event) {
	if (m_isActive) {
		IInputManager::TEvent eventType = event.getType();
		if (eventType == IInputManager::TEvent::EMouse) {
			const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);
			if (mouseEvent.mouseButtonAction == mouseEvent.AMove) {
				vec2 targetPos = vmake(mouseEvent.x, mouseEvent.y);
				m_target->setPos(targetPos);

				MessageGetTransform messagePos;
				m_owner->receiveMessage(&messagePos);
				MessageAimDirection messageAimDirection;
				messageAimDirection.direction = vnorm(vsub(targetPos, messagePos.pos));
				m_owner->receiveMessage(&messageAimDirection);
			}
		}
	}

	return true;
}