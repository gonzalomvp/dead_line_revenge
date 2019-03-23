#include "common/stdafx.h"
#include "component.h"

#include "engine/graphics_engine.h"
#include "engine/sprite.h"
#include "engine/text.h"
#include "engine/sound_engine.h"
#include "gui/string_manager.h"
#include "scenes/world.h"
#include "entities/entity.h"
#include "entities/entities_factory.h"
#include "messages/message.h"

#include <algorithm>



//=============================================================================
// Component class
//=============================================================================
void Component::init() {
	m_owner->addComponent(this);
	if (m_activationDelay == 0) {
		activate();
	}
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
// CTransformComponent class
//=============================================================================


//=============================================================================
// CLifeComponent class
//=============================================================================

//=============================================================================
// CMovementComponent class
//=============================================================================

//=============================================================================
// CRenderableComponent class
//=============================================================================

//=============================================================================
// ComponentPlayerController class
//=============================================================================

//=============================================================================
// ComponentWeapon class
//=============================================================================

ComponentWeapon::TWeaponInfo ComponentWeapon::s_aWeaponInfo[] =
{
#define REG_WEAPON(val, name) \
	{E##val, name},
#include "REG_WEAPONS.h"
#undef REG_WEAPON
};

void ComponentWeapon::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	if (m_fireTimer < m_mWeaponData.fireRate) {
		++m_fireTimer;
	}
	if (m_reloadTimer <= m_mWeaponData.reloadTime) {
		++m_reloadTimer;
		if (m_reloadTimer == m_mWeaponData.reloadTime) {
			m_currentBullets = m_mWeaponData.capacity;
			m_isFiring       = false;
		}
	}

	if (m_isFiring && m_remoteBullet) {
		MessageDestroy msgDestroy;
		m_remoteBullet->receiveMessage(&msgDestroy);
		m_remoteBullet = nullptr;
		m_isFiring = false;
		m_currentBullets = m_mWeaponData.capacity;
	}

	else if (m_isFiring && m_fireTimer >= m_mWeaponData.fireRate && m_reloadTimer >= m_mWeaponData.reloadTime && m_currentBullets > 0) {
		m_fireTimer = 0;
		--m_currentBullets;
		if (m_currentBullets == 0) {
			m_reloadTimer = 0;
		}
		if (!m_mWeaponData.isAutomatic) {
			m_isFiring = false;
		}
		
		MessageGetTransform messageGetTranform;
		m_owner->receiveMessage(&messageGetTranform);

		switch (m_mWeaponData.type) {
			case ESHOTGUN: {
				vec2 bulletDir = m_aimDirection;
				g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), bulletDir, m_mWeaponData.bulletSpeed, m_mWeaponData.bulletDamage, m_mWeaponData.bulletLife, m_mWeaponData.bulletRange, m_mWeaponData.isExplossive, m_mWeaponData.isBouncy, m_owner->getType(), "data/shotgunBullet.png"));
				float angle = vangle(m_aimDirection);
				angle += SHOTGUN_DISP_ANGLE;
				bulletDir = vunit(DEG2RAD(angle));
				g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), bulletDir, m_mWeaponData.bulletSpeed, m_mWeaponData.bulletDamage, m_mWeaponData.bulletLife, m_mWeaponData.bulletRange, m_mWeaponData.isExplossive, m_mWeaponData.isBouncy, m_owner->getType(), "data/shotgunBullet.png"));
				angle = vangle(m_aimDirection);
				angle -= SHOTGUN_DISP_ANGLE;
				bulletDir = vunit(DEG2RAD(angle));
				g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), bulletDir, m_mWeaponData.bulletSpeed, m_mWeaponData.bulletDamage, m_mWeaponData.bulletLife, m_mWeaponData.bulletRange, m_mWeaponData.isExplossive, m_mWeaponData.isBouncy, m_owner->getType(), "data/shotgunBullet.png"));
				break;
			}
			case EMINES: {
				g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(20.0f, 20.0f), m_aimDirection, m_mWeaponData.bulletSpeed, m_mWeaponData.bulletDamage, m_mWeaponData.bulletLife, m_mWeaponData.bulletRange, m_mWeaponData.isExplossive, m_mWeaponData.isBouncy, Entity::EMINE, "data/mine.png"));
				break;
			}
			case EC4: {
				m_remoteBullet = g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(20.0f, 20.0f), m_aimDirection, m_mWeaponData.bulletSpeed, m_mWeaponData.bulletDamage, m_mWeaponData.bulletLife, m_mWeaponData.bulletRange, m_mWeaponData.isExplossive, m_mWeaponData.isBouncy, m_owner->getType(), "data/c4.png");
				g_pWorld->addEntity(m_remoteBullet);
				break;
			}
			case EROCKETLAUNCHER: {
				g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(15.0f, 15.0f), m_aimDirection, m_mWeaponData.bulletSpeed, m_mWeaponData.bulletDamage, m_mWeaponData.bulletLife, m_mWeaponData.bulletRange, m_mWeaponData.isExplossive, m_mWeaponData.isBouncy, m_owner->getType(), "data/rocket.png"));
				break;
			}
			case ENUCLEARBOMB: {
				g_pWorld->addEntity(g_pEntitiesFactory->createExplossion(messageGetTranform.pos, vmake(20.0f, 20.0f), vmake(8.0f, 8.0f), 100, Entity::ENUCLEAREXPLOSSION));
				break;
			}
			default: {
				if (m_mWeaponData.numBullets > 1) {
					for (size_t i = 0; i < m_mWeaponData.numBullets; i++) {
						float angle = vangle(m_aimDirection) + (i * 360.f / m_mWeaponData.numBullets);
						g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), vunit(DEG2RAD(angle)), m_mWeaponData.bulletSpeed, m_mWeaponData.bulletDamage, m_mWeaponData.bulletLife, m_mWeaponData.bulletRange, m_mWeaponData.isExplossive, m_mWeaponData.isBouncy, m_owner->getType(), "data/bullet.png"));
					}
				}
				else {
					g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), m_aimDirection, m_mWeaponData.bulletSpeed, m_mWeaponData.bulletDamage, m_mWeaponData.bulletLife, m_mWeaponData.bulletRange, m_mWeaponData.isExplossive, m_mWeaponData.isBouncy, m_owner->getType(), "data/bullet.png"));
				}
				
				break;
			}
		}

		if (m_mWeaponData.soundFile != "") {
			g_pSoundEngine->playSound(m_mWeaponData.soundFile.c_str());
		}

		MessageFire messageFire;
		messageFire.isFiring = m_isFiring;
		messageFire.isFireDone = true;
		m_owner->receiveMessage(&messageFire);
	}
}

void ComponentWeapon::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageWeaponChange* msgWeaponChange = dynamic_cast<MessageWeaponChange*>(message);
	if (msgWeaponChange) {
		m_mWeaponData = msgWeaponChange->weaponData;
		m_isFiring = false;
		m_currentBullets = m_mWeaponData.capacity;
		m_fireTimer = m_mWeaponData.fireRate;
		m_reloadTimer = m_mWeaponData.reloadTime;
	}
	else {
		MessageFire* msgFire = dynamic_cast<MessageFire*>(message);
		if (msgFire) {
			m_isFiring = msgFire->isFiring;
		}
		else {
			MessageSetAimDirection* messageSetAimDirection = dynamic_cast<MessageSetAimDirection*>(message);
			if (messageSetAimDirection) {
				m_aimDirection = messageSetAimDirection->direction;
			}
			else {
				MessageGetAimDirection* messageGetAimDirection = dynamic_cast<MessageGetAimDirection*>(message);
				if (messageGetAimDirection) {
					messageGetAimDirection->direction = m_aimDirection;
				}
				else {
					MessageAmmoInfo* msgAmmoInfo = dynamic_cast<MessageAmmoInfo*>(message);
					if (msgAmmoInfo) {
						msgAmmoInfo->currentAmmo = m_currentBullets;
						msgAmmoInfo->totalAmmo = m_mWeaponData.capacity;
						msgAmmoInfo->reloadPercent = m_reloadTimer * 1.0f / m_mWeaponData.reloadTime;
						if (msgAmmoInfo->reloadPercent > 1.0f) {
							msgAmmoInfo->reloadPercent = 1.0f;
						}
					}
					else {
						MessageReload* msgReload = dynamic_cast<MessageReload*>(message);
						if (msgReload && m_currentBullets < m_mWeaponData.capacity && m_reloadTimer >= m_mWeaponData.reloadTime) {
							m_isFiring = false;
							m_reloadTimer = 0;
						}
					}
				}
			}
		}
	}
}

ComponentWeapon::EType ComponentWeapon::getWeaponTypeByName(const std::string& name) {
	ComponentWeapon::EType etype = EInvalid;
	int i = 0;
	while ((etype == EInvalid) && (i < NUM_WEAPON_TYPES))
	{
		if (name == s_aWeaponInfo[i].sName) {
			etype = s_aWeaponInfo[i].eType;
		}
		i++;
	}
	return etype;
}

//=============================================================================
// ComponentExplossive class
//=============================================================================


//=============================================================================
// ComponentAIMelee class
//=============================================================================


//=============================================================================
// ComponentAIEvade class
//=============================================================================


//=============================================================================
// ComponentAIFire class
//=============================================================================


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

	MessageCheckCollision* msgCheckCollision = dynamic_cast<MessageCheckCollision*>(message);
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
}

//=============================================================================
// ComponentPoints class
//=============================================================================
void ComponentPoints::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageDestroy* msgDestroy = dynamic_cast<MessageDestroy*>(message);
	if (msgDestroy) {
		g_pWorld->addPoints(m_owner->getType());
	}
}

//=============================================================================
// ComponentWeaponPickup class
//=============================================================================
void ComponentWeaponPickup::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageDestroy* msgDestroy = dynamic_cast<MessageDestroy*>(message);
	if (msgDestroy) {
		MessageWeaponChange msgWeapon;
		msgWeapon.weaponData = m_mWeaponData;
		g_pWorld->getPlayer()->receiveMessage(&msgWeapon);

		std::string hudMessage = g_pStringManager->getText("LTEXT_GUI_PICKUP_MESSAGE");
		switch (m_mWeaponData.type)
		{
#define REG_WEAPON(val, name) \
			case ComponentWeapon::E##val: \
				hudMessage += g_pStringManager->getText("LTEXT_GUI_"#val"_MESSAGE"); \
				break;
#include "REG_WEAPONS.h"
#undef REG_WEAPON
			default:
				break;
		}

		g_pWorld->addEntity(g_pEntitiesFactory->createHUDMessage(hudMessage, vmake((WORLD_WIDTH / 2) - g_pStringManager->calculateTextHalfWidth(hudMessage), 20), 100));
	}
}

//=============================================================================
// ComponentHUDMessage class
//=============================================================================
ComponentHUDMessage::~ComponentHUDMessage() {
	DELETE(m_message);
}

void ComponentHUDMessage::init() {
	Component::init();
	m_message = NEW(CText, m_messageText, m_pos, 1);
	g_pGraphicsEngine->addGfxEntity(m_message);
}

//=============================================================================
// ComponentHUD class
//=============================================================================
ComponentHUD::~ComponentHUD() {
	for (size_t i = 0; i < m_gfxEntities.size(); ++i) {
		DELETE(m_gfxEntities[i]);
	}
	if (g_pInputManager) {
		g_pInputManager->unregisterEvent(this, IInputManager::EEventType::EMouseMove);
	}
}

void ComponentHUD::init() {
	Component::init();
	
	// Life HUD
	CText* title = NEW(CText, "LTEXT_GUI_LIFE_HUD", vmake(20, 450), 4);
	g_pGraphicsEngine->addGfxEntity(title);
	m_gfxEntities.push_back(title);
	float titleEndPos = title->getPos().x + g_pStringManager->getText(title->getText()).length() * 16.0f;
	m_life = NEW(CText, "0", vmake(title->getPos().x, 430), 4);
	g_pGraphicsEngine->addGfxEntity(m_life);
	m_gfxEntities.push_back(m_life);

	// Score HUD
	title = NEW(CText, "LTEXT_GUI_SCORE_HUD", vmake(titleEndPos + 20, 450), 4);
	g_pGraphicsEngine->addGfxEntity(title);
	m_gfxEntities.push_back(title);
	titleEndPos = title->getPos().x + g_pStringManager->getText(title->getText()).length() * 16.0f;
	m_uScore = NEW(CText, "0", vmake(title->getPos().x, 430), 4);
	g_pGraphicsEngine->addGfxEntity(m_uScore);
	m_gfxEntities.push_back(m_uScore);

	// AMMO HUD
	title = NEW(CText, "LTEXT_GUI_AMMO_HUD", vmake(titleEndPos + 20, 450), 4);
	g_pGraphicsEngine->addGfxEntity(title);
	m_gfxEntities.push_back(title);
	m_ammo = NEW(CText, "0/0", vmake(title->getPos().x, 430), 4);
	g_pGraphicsEngine->addGfxEntity(m_ammo);
	m_gfxEntities.push_back(m_ammo);

	// Target HUD
	m_target = NEW(CSprite, g_pGraphicsEngine->getTexture("data/target.png"), vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), 0.0f, 1.0f, 4);
	m_target->setSize(vmake(36, 36));
	g_pGraphicsEngine->addGfxEntity(m_target);
	m_gfxEntities.push_back(m_target);

	// Reload Animation
	m_reloadAnim = NEW(CSprite, g_pGraphicsEngine->getTexture("data/reload.png"), vmake(0.0f, 0.0f), vmake(0.0f, 0.0f), 0.0f, 1.0f, 4);
	g_pGraphicsEngine->addGfxEntity(m_reloadAnim);
	m_gfxEntities.push_back(m_reloadAnim);

	g_pInputManager->registerEvent(this, IInputManager::EEventType::EMouseMove);
}

void ComponentHUD::run(float deltaTime) {
	Component::run(deltaTime);
	MessageGetLife msgLife;
	m_owner->receiveMessage(&msgLife);
	m_life->setText(std::to_string(msgLife.currentLife));
	
	m_uScore->setText(std::to_string(g_pWorld->getScore()));

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

bool ComponentHUD::onEvent(const IInputManager::CEvent& event) {
	if (m_isActive) {
		IInputManager::EEventType eventType = event.getType();
		if (eventType == IInputManager::EEventType::EMouseMove) {
			const IInputManager::CMouseEvent mouseEvent = *static_cast<const IInputManager::CMouseEvent*>(&event);
			vec2 targetPos = mouseEvent.getPos();
			m_target->setPos(targetPos);

			MessageGetTransform messagePos;
			m_owner->receiveMessage(&messagePos);
			MessageSetAimDirection messageSetAimDirection;
			messageSetAimDirection.direction = vnorm(vsub(targetPos, messagePos.pos));
			messageSetAimDirection.changeAngle = true;
			m_owner->receiveMessage(&messageSetAimDirection);
		}
	}
	return true;
}

//=============================================================================
// Collision check utils
//=============================================================================
