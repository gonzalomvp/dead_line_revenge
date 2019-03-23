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
		m_mWeaponData = g_pEntitiesFactory->getWeaponDef(msgWeaponChange->eWeaponType);
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


//=============================================================================
// ComponentPoints class
//=============================================================================


//=============================================================================
// ComponentWeaponPickup class
//=============================================================================


//=============================================================================
// ComponentHUDMessage class
//=============================================================================


//=============================================================================
// ComponentHUD class
//=============================================================================


//=============================================================================
// Collision check utils
//=============================================================================
