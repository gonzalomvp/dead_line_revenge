#include "../../common/stdafx.h"
#include "component.h"
#include "../entity.h"
#include "../message.h"
#include "../../scenes/world.h"
#include "../../scenes/level.h"
#include "../../gui/string_manager.h"

#define EVASION_ANGLE         45.0f
#define EVASION_MIN_WALL_DIST 20.0f

//=============================================================================
// Component class
//=============================================================================
Component::Component(Entity* owner) : m_owner(owner), m_isActive(false) {
	//m_owner->addComponent(this);
}

void Component::init() {
	m_owner->addComponent(this);
	activate();
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

//=============================================================================
// ComponentTransform class
//=============================================================================
void ComponentTransform::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageSetTransform *msgSetTransform = dynamic_cast<MessageSetTransform*>(message);
	if (msgSetTransform) {
		m_pos = msgSetTransform->pos;
		m_size = msgSetTransform->size;
		bool outOfBounds = false;

		if (m_pos.x > SCR_WIDTH) {
			m_pos.x = SCR_WIDTH;
			outOfBounds = true;
		}
		else if (m_pos.x < 0) {
			m_pos.x = 0;
			outOfBounds = true;
		}
		if (m_pos.y > SCR_HEIGHT) {
			m_pos.y = SCR_HEIGHT;
			outOfBounds = true;
		}
		else if (m_pos.y < 0) {
			m_pos.y = 0;
			outOfBounds = true;
		}
		MessageTransformChanged msgTransformChanged;
		msgTransformChanged.pos = m_pos;
		msgTransformChanged.size = m_size;
		m_owner->receiveMessage(&msgTransformChanged);

		if (outOfBounds) {
			MessageCollision msgCollision;
			msgCollision.deltaLife = 0;
			msgCollision.faction = EBounds;
			m_owner->receiveMessage(&msgCollision);
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
	m_hitTimer  = 0;
}

void ComponentLife::run() {
	if (!m_isActive)
		return;

	if (m_invencibleTime != 0) {
		++m_hitTimer;
		if (m_hitTimer > m_invencibleTime) {
			m_hitTimer = m_invencibleTime;
		}
	}

	if (m_timeToLive != 0) {
		++m_lifeTimer;
		if (m_lifeTimer >= m_timeToLive) {
			MessageDestroy msgDestroy;
			m_owner->receiveMessage(&msgDestroy);
			m_owner->deactivate();
			g_world->removeEntity(m_owner);
		}
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
		if (msgChangeLife && (m_life != -1) && (m_hitTimer >= m_invencibleTime)) {
			m_life += msgChangeLife->deltaLife;
			m_hitTimer = 0;
			if (m_life <= 0) {
				MessageDestroy msgDestroy;
				m_owner->receiveMessage(&msgDestroy);
				m_owner->deactivate();
				g_world->removeEntity(m_owner);
			}
		}
	}
}

//=============================================================================
// ComponentInertialMove class
//=============================================================================
void ComponentInertialMove::run() {
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

void ComponentInertialMove::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageAddMovement *msgAddMovement = dynamic_cast<MessageAddMovement*>(message);
	if (msgAddMovement) {
		m_direction = vadd(m_direction, msgAddMovement->dir);
	}
}

//=============================================================================
// ComponentRenderable class
//=============================================================================
ComponentRenderable::ComponentRenderable(Entity* owner, const char* texture, float alpha, const char* hitTexture, int hitTime) : Component(owner), m_texture(texture), m_alpha(alpha), m_hitTexture(hitTexture), m_hitTime(hitTime) {
	m_hitTimer = 0;
}

ComponentRenderable::~ComponentRenderable() {
	g_graphicsEngine->removeGfxEntity(m_sprite);
}

void ComponentRenderable::init() {
	Component::init();
	m_sprite = new Sprite(g_graphicsEngine->getTexture(m_texture), 2, m_alpha);
	g_graphicsEngine->addGfxEntity(m_sprite);
}

void ComponentRenderable::run() {
	if (!m_isActive)
		return;

	MessageGetTransform msgGetTransform;
	m_owner->receiveMessage(&msgGetTransform);
	m_sprite->setPos(msgGetTransform.pos);
	m_sprite->setSize(msgGetTransform.size);

	if (m_hitTimer <= m_hitTime) {
		++m_hitTimer;
		if (m_hitTimer == m_hitTime) {
			m_sprite->setTexture(g_graphicsEngine->getTexture(m_texture));
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
		if (msgChangeLife && msgChangeLife->deltaLife < 0 && m_hitTexture) {
			m_sprite->setTexture(g_graphicsEngine->getTexture(m_hitTexture));
			m_hitTimer = 0;
		}
	}
}

//=============================================================================
// ComponentPlayerController class
//=============================================================================
void ComponentPlayerController::init() {
	Component::init();
	g_inputManager->registerEvent(this, IInputManager::TEvent::EKey, 0);
}

ComponentPlayerController::~ComponentPlayerController() {
	g_inputManager->unregisterEvent(this);
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
ComponentWeapon::ComponentWeapon(Entity* owner, TWeapon type, int fireRate, int reloadTime, int bullets, int bulletSpeed, int bulletDamage, int bulletRange, bool isAutomatic, const char* soundFilename) : Component(owner), m_type(type), m_fireRate(fireRate), m_reloadTime(reloadTime), m_bullets(bullets), m_bulletSpeed(bulletSpeed), m_bulletDamage(bulletDamage), m_bulletRange(bulletRange), m_isAutomatic(isAutomatic), m_soundFilename(soundFilename) {
	m_aimDirection   = vmake(0.0f, 0.0f);
	m_currentBullets = m_bullets;
	m_isFiring       = false;
	m_soundId        = 0;
	m_fireTimer      = fireRate;
	m_reloadTimer    = reloadTime;
}

void ComponentWeapon::init() {
	Component::init();
	if (m_soundFilename) {
		m_soundId = CORE_LoadWav(m_soundFilename);
	}
}

void ComponentWeapon::run() {
	if (!m_isActive)
		return;

	if (m_fireTimer < m_fireRate) {
		++m_fireTimer;
	}
	if (m_reloadTimer <= m_reloadTime) {
		++m_reloadTimer;
		if (m_reloadTimer == m_reloadTime) {
			m_currentBullets = m_bullets;
		}
	}

	if (m_isFiring && m_fireTimer >= m_fireRate && m_reloadTimer >= m_reloadTime && m_currentBullets != 0) {
		m_fireTimer = 0;
		if (m_currentBullets >= 0) {
			--m_currentBullets;
			if (m_currentBullets == 0) {
				m_reloadTimer = 0;
				m_isFiring = false;
			}
		}
		
		MessageGetTransform messageGetTranform;
		m_owner->receiveMessage(&messageGetTranform);
		MessageGetCollider msgGetCollider;
		m_owner->receiveMessage(&msgGetCollider);

		switch (m_type)
		{
			case EShotgun:
				createShotgunBullets(messageGetTranform.pos, m_aimDirection, m_bulletSpeed, m_bulletDamage, m_bulletRange, msgGetCollider.faction);
				break;
			case EMines:
				createMine(this, messageGetTranform.pos, m_bulletDamage, msgGetCollider.faction);
				deactivate();
				break;
			default:
				g_world->addEntity(createBullet(messageGetTranform.pos, m_aimDirection, m_bulletSpeed, m_bulletDamage, m_bulletRange, msgGetCollider.faction));
				break;
		}
		
		
		if (!m_isAutomatic) {
			m_isFiring = false;
		}

		//Sound (mover a un sound manager)
		if (g_settings.sfx && m_soundId) {
			CORE_PlayMusic(m_soundId);
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
		m_type = msgWeaponChange->weapon;
		switch (m_type) {
			case ERevolver:
				m_fireRate = 20;
				m_reloadTime = 60;
				m_bullets = 6;
				m_bulletSpeed = 6;
				m_bulletDamage = -2;
				m_bulletRange = 60;
				m_isAutomatic = false;
				m_soundFilename = "data/shot.wav";
				break;
			case EMachinegun:
				m_fireRate = 10;
				m_reloadTime = 80;
				m_bullets = 20;
				m_bulletSpeed = 8;
				m_bulletDamage = -1;
				m_bulletRange = 0;
				m_isAutomatic = true;
				m_soundFilename = "data/shot.wav";
				break;
			case EShotgun:
				m_fireRate = 20;
				m_reloadTime = 40;
				m_bullets = 2;
				m_bulletSpeed = 4;
				m_bulletDamage = -1;
				m_bulletRange = 40;
				m_isAutomatic = false;
				m_soundFilename = "data/shotgun.wav";
				break;
			case EMines:
				m_fireRate = 20;
				m_reloadTime = 40;
				m_bullets = 2;
				m_bulletSpeed = 4;
				m_bulletDamage = -1;
				m_bulletRange = 30;
				m_isAutomatic = false;
				m_soundFilename = "data/mine.wav";
				break;
		}
		m_isFiring = false;
		m_currentBullets = m_bullets;
		m_fireTimer = m_fireRate;
		m_reloadTimer = m_reloadTime;
		if (m_soundFilename) {
			m_soundId = CORE_LoadWav(m_soundFilename);
		}
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
					msgAmmoInfo->totalAmmo = m_bullets;
					msgAmmoInfo->reloadPercent = m_reloadTimer * 1.0f / m_reloadTime;
					if (msgAmmoInfo->reloadPercent > 1.0f) {
						msgAmmoInfo->reloadPercent = 1.0f;
					}
				}
				else {
					MessageReload *msgReload = dynamic_cast<MessageReload*>(message);
					if (msgReload && m_currentBullets < m_bullets && m_reloadTimer >= m_reloadTime) {
						m_reloadTimer = 0;
						m_isFiring = false;
					}
				}
			}
		}
	}
}

//=============================================================================
// ComponentExplossion class
//=============================================================================
void ComponentExplossion::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageDestroy *msgDestroy = dynamic_cast<MessageDestroy*>(message);
	if (msgDestroy) {
		MessageGetTransform messageSelfPos;
		m_owner->receiveMessage(&messageSelfPos);
		createExplossion(messageSelfPos.pos, vmake(100, 100));
	}
}


//Me lo salto por si lo muevo al HUD
//=============================================================================
// C_Target class
//=============================================================================
C_Target::C_Target(Entity* owner, char* texture) : Component(owner){
	m_sprite = new Sprite(g_graphicsEngine->getTexture(texture), 1);
	m_sprite->setSize(vmake(50, 50));
	g_graphicsEngine->addGfxEntity(m_sprite);
	g_inputManager->registerEvent(this, IInputManager::TEvent::EKey, 0);
}

C_Target::~C_Target() {
	g_graphicsEngine->removeGfxEntity(m_sprite);
	g_inputManager->unregisterEvent(this);
}


//void C_Target::receiveMessage(Message* message) {
//	MessageGetTargetPos *msgTarget = dynamic_cast<MessageGetTargetPos*>(message);
//	if (msgTarget) {
//		msgTarget->pos = m_sprite->getPos();
//	}
//}

bool C_Target::onEvent(const IInputManager::Event& event) {
	if (m_isActive) {
		IInputManager::TEvent eventType = event.getType();
		if (eventType == IInputManager::TEvent::EMouse) {
			const IInputManager::MouseEvent mouseEvent = *static_cast<const IInputManager::MouseEvent*>(&event);
			if (mouseEvent.mouseButtonAction == mouseEvent.AMove) {
				vec2 targetPos = vmake(mouseEvent.x, mouseEvent.y);
				m_sprite->setPos(targetPos);

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

void C_Target::activate() {
	if (!m_isActive) {
		Component::activate();
		g_graphicsEngine->addGfxEntity(m_sprite);
	}
	
}

void C_Target::deactivate() {
	if (m_isActive) {
		Component::deactivate();
		g_graphicsEngine->removeGfxEntity(m_sprite);
	}
	
}

//=============================================================================
// ComponentAIMelee class
//=============================================================================
void ComponentAIMelee::run() {
	if (!m_isActive)
		return;

	MessageGetTransform messageSelfPos;
	m_owner->receiveMessage(&messageSelfPos);
	MessageGetTransform messagePlayerPos;
	m_player->receiveMessage(&messagePlayerPos);
	vec2 direction = vsub(messagePlayerPos.pos, messageSelfPos.pos);

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
void ComponentAIEvade::run() {
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
	// Ver si se puede hacer simplemente analizando las coordenadas x, y de la posicion y las posiciones limites de las lineas SCR_HEIGHT, SCR_WIDTH, 0 ,0
	// por ejemplo (SCR_HEIGHT / componente y del movimiento) = cercania
	vec2 moveDir = vunit(DEG2RAD(angle));
	vec2 intersection1;
	vec2 intersection2;

	if (moveDir.y > 0)
		LineLineIntersect(position, vadd(position, moveDir), vmake(0, SCR_HEIGHT), vmake(SCR_WIDTH, SCR_HEIGHT), intersection1);
	else
		LineLineIntersect(position, vadd(position, moveDir), vmake(0, 0), vmake(SCR_WIDTH, 0), intersection1);

	if (moveDir.x > 0)
		LineLineIntersect(position, vadd(position, moveDir), vmake(SCR_WIDTH, 0), vmake(SCR_WIDTH, SCR_HEIGHT), intersection2);
	else
		LineLineIntersect(position, vadd(position, moveDir), vmake(0, 0), vmake(0, SCR_HEIGHT), intersection2);

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

void ComponentAIFire::run() {
	if (!m_isActive)
		return;

	if (m_player) {
		MessageGetTransform messageSelfPos;
		m_owner->receiveMessage(&messageSelfPos);
		MessageGetTransform messagePlayerPos;
		m_player->receiveMessage(&messagePlayerPos);
		MessageAimDirection messageAimDirection;
		messageAimDirection.direction = vnorm(vsub(messagePlayerPos.pos, messageSelfPos.pos));
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
void ComponentCollider::run() {
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
	
	MessageGetCollider *msgCollider = dynamic_cast<MessageGetCollider*>(message);
	if (msgCollider) {
		msgCollider->type      = m_type;
		msgCollider->faction   = m_faction;
		msgCollider->center    = m_center;
		msgCollider->size      = m_size;
		msgCollider->deltaLife = m_deltaLife;
	}
	else {
		MessageCollision *msgCollision = dynamic_cast<MessageCollision*>(message);
		if (msgCollision && m_faction != ENeutral && msgCollision->faction != ENeutral) {
			MessageChangeLife mgsChangeLife;
			mgsChangeLife.deltaLife = msgCollision->deltaLife;
			m_owner->receiveMessage(&mgsChangeLife);
		}
	}
}

//=============================================================================
// ComponentPoints class
//=============================================================================
void ComponentPoints::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageDestroy *msgDestroy = dynamic_cast<MessageDestroy*>(message);
	if (msgDestroy) {
		g_world->m_level->addPoints(m_points);
	}
}

//Revisar el tema colisiones y facciones
//=============================================================================
// ComponentWeaponPickup class
//=============================================================================
void ComponentWeaponPickup::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageCollision *msgCollision = dynamic_cast<MessageCollision*>(message);
	if (msgCollision && g_world->getPlayer() == msgCollision->other) {
		MessageWeaponChange msgWeapon;
		msgWeapon.weapon = m_weapon;
		msgCollision->other->receiveMessage(&msgWeapon);
		
		MessageChangeLife mgsChangeLife;
		mgsChangeLife.deltaLife = msgCollision->deltaLife;
		m_owner->receiveMessage(&mgsChangeLife);

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
		}

		createHUDMessage(hudMessage, vmake((SCR_WIDTH / 2) - (hudMessage.length() / 2.0f * 16), SCR_HEIGHT * 0.8f), 100);
	}
}

//=============================================================================
// ComponentWeaponReactivator class
//=============================================================================
void ComponentWeaponReactivator::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageDestroy *msgDestroy = dynamic_cast<MessageDestroy*>(message);
	if (msgDestroy && m_weapon) {
		m_weapon->activate();
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
// C_HUDLife class
//=============================================================================
C_HUDLife::C_HUDLife(Entity* owner, Entity* player) : Component(owner), m_player(player) {
	m_hudLife = new Text("", 1, vmake(20, 20));
	if (m_player) {
		MessageGetLife msgLife;
		m_player->receiveMessage(&msgLife);
		m_hudLife->setText(std::to_string(msgLife.currentLife));
	}
	g_graphicsEngine->addGfxEntity(m_hudLife);
}

C_HUDLife::~C_HUDLife() {
	g_graphicsEngine->removeGfxEntity(m_hudLife);
}

void C_HUDLife::run() {
	if (m_player) {
		MessageGetLife msgLife;
		m_player->receiveMessage(&msgLife);
		m_hudLife->setText(std::to_string(msgLife.currentLife));
	}
}