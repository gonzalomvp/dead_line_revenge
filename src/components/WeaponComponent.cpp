#include "common/stdafx.h"
#include "WeaponComponent.h"

#include "engine/sound_engine.h"
#include "entities/entity.h"
#include "entities/entities_factory.h"
#include "messages/message.h"
#include "scenes/world.h"

#define SHOTGUN_DISP_ANGLE 15.0f

CWeaponComponent::TWeaponInfo CWeaponComponent::s_aWeaponInfo[] =
{
#define REG_WEAPON(val, name) \
	{E##val, name},
#include "REG_WEAPONS.h"
#undef REG_WEAPON
};

void CWeaponComponent::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	if (m_fireTimer < m_mWeaponData.iFireRate) {
		++m_fireTimer;
	}
	if (m_reloadTimer <= m_mWeaponData.iReloadTime) {
		++m_reloadTimer;
		if (m_reloadTimer == m_mWeaponData.iReloadTime) {
			m_currentBullets = m_mWeaponData.iCapacity;
			m_isFiring = false;
		}
	}

	if (m_isFiring && m_remoteBullet) {
		MessageDestroy msgDestroy;
		m_remoteBullet->receiveMessage(&msgDestroy);
		m_remoteBullet = nullptr;
		m_isFiring = false;
		m_currentBullets = m_mWeaponData.iCapacity;
	}

	else if (m_isFiring && m_fireTimer >= m_mWeaponData.iFireRate && m_reloadTimer >= m_mWeaponData.iReloadTime && m_currentBullets > 0) {
		m_fireTimer = 0;
		--m_currentBullets;
		if (m_currentBullets == 0) {
			m_reloadTimer = 0;
		}
		if (!m_mWeaponData.bIsAutomatic) {
			m_isFiring = false;
		}

		MessageGetTransform messageGetTranform;
		m_owner->receiveMessage(&messageGetTranform);

		switch (m_mWeaponData.eType) {
		case ESHOTGUN: {
			vec2 bulletDir = m_aimDirection;
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), bulletDir, m_mWeaponData.fBulletSpeed, m_mWeaponData.iBulletDamage, m_mWeaponData.iBulletLife, m_mWeaponData.iBulletRange, m_mWeaponData.bIsExplossive, m_mWeaponData.bIsBouncy, m_owner->getType(), "data/shotgunBullet.png"));
			float angle = vangle(m_aimDirection);
			angle += SHOTGUN_DISP_ANGLE;
			bulletDir = vunit(DEG2RAD(angle));
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), bulletDir, m_mWeaponData.fBulletSpeed, m_mWeaponData.iBulletDamage, m_mWeaponData.iBulletLife, m_mWeaponData.iBulletRange, m_mWeaponData.bIsExplossive, m_mWeaponData.bIsBouncy, m_owner->getType(), "data/shotgunBullet.png"));
			angle = vangle(m_aimDirection);
			angle -= SHOTGUN_DISP_ANGLE;
			bulletDir = vunit(DEG2RAD(angle));
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), bulletDir, m_mWeaponData.fBulletSpeed, m_mWeaponData.iBulletDamage, m_mWeaponData.iBulletLife, m_mWeaponData.iBulletRange, m_mWeaponData.bIsExplossive, m_mWeaponData.bIsBouncy, m_owner->getType(), "data/shotgunBullet.png"));
			break;
		}
		case EMINES: {
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(20.0f, 20.0f), m_aimDirection, m_mWeaponData.fBulletSpeed, m_mWeaponData.iBulletDamage, m_mWeaponData.iBulletLife, m_mWeaponData.iBulletRange, m_mWeaponData.bIsExplossive, m_mWeaponData.bIsBouncy, Entity::EMINE, "data/mine.png"));
			break;
		}
		case EC4: {
			m_remoteBullet = g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(20.0f, 20.0f), m_aimDirection, m_mWeaponData.fBulletSpeed, m_mWeaponData.iBulletDamage, m_mWeaponData.iBulletLife, m_mWeaponData.iBulletRange, m_mWeaponData.bIsExplossive, m_mWeaponData.bIsBouncy, m_owner->getType(), "data/c4.png");
			g_pWorld->addEntity(m_remoteBullet);
			break;
		}
		case EROCKETLAUNCHER: {
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(15.0f, 15.0f), m_aimDirection, m_mWeaponData.fBulletSpeed, m_mWeaponData.iBulletDamage, m_mWeaponData.iBulletLife, m_mWeaponData.iBulletRange, m_mWeaponData.bIsExplossive, m_mWeaponData.bIsBouncy, m_owner->getType(), "data/rocket.png"));
			break;
		}
		case ENUCLEARBOMB: {
			g_pWorld->addEntity(g_pEntitiesFactory->createExplossion(messageGetTranform.pos, vmake(20.0f, 20.0f), vmake(8.0f, 8.0f), 100, Entity::ENUCLEAREXPLOSSION));
			break;
		}
		default: {
			if (m_mWeaponData.uNumBullets > 1) {
				for (size_t i = 0; i < m_mWeaponData.uNumBullets; i++) {
					float angle = vangle(m_aimDirection) + (i * 360.f / m_mWeaponData.uNumBullets);
					g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), vunit(DEG2RAD(angle)), m_mWeaponData.fBulletSpeed, m_mWeaponData.iBulletDamage, m_mWeaponData.iBulletLife, m_mWeaponData.iBulletRange, m_mWeaponData.bIsExplossive, m_mWeaponData.bIsBouncy, m_owner->getType(), "data/bullet.png"));
				}
			}
			else {
				g_pWorld->addEntity(g_pEntitiesFactory->createBullet(messageGetTranform.pos, vmake(10.0f, 10.0f), m_aimDirection, m_mWeaponData.fBulletSpeed, m_mWeaponData.iBulletDamage, m_mWeaponData.iBulletLife, m_mWeaponData.iBulletRange, m_mWeaponData.bIsExplossive, m_mWeaponData.bIsBouncy, m_owner->getType(), "data/bullet.png"));
			}

			break;
		}
		}

		if (m_mWeaponData.sSoundFile != "") {
			g_pSoundEngine->playSound(m_mWeaponData.sSoundFile.c_str());
		}

		MessageFire messageFire;
		messageFire.isFiring = m_isFiring;
		messageFire.isFireDone = true;
		m_owner->receiveMessage(&messageFire);
	}
}

void CWeaponComponent::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageWeaponChange* msgWeaponChange = dynamic_cast<MessageWeaponChange*>(message);
	if (msgWeaponChange) {
		m_mWeaponData = g_pEntitiesFactory->getWeaponDef(msgWeaponChange->eWeaponType);
		m_isFiring = false;
		m_currentBullets = m_mWeaponData.iCapacity;
		m_fireTimer = m_mWeaponData.iFireRate;
		m_reloadTimer = m_mWeaponData.iReloadTime;
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
						msgAmmoInfo->totalAmmo = m_mWeaponData.iCapacity;
						msgAmmoInfo->reloadPercent = m_reloadTimer * 1.0f / m_mWeaponData.iReloadTime;
						if (msgAmmoInfo->reloadPercent > 1.0f) {
							msgAmmoInfo->reloadPercent = 1.0f;
						}
					}
					else {
						MessageReload* msgReload = dynamic_cast<MessageReload*>(message);
						if (msgReload && m_currentBullets < m_mWeaponData.iCapacity && m_reloadTimer >= m_mWeaponData.iReloadTime) {
							m_isFiring = false;
							m_reloadTimer = 0;
						}
					}
				}
			}
		}
	}
}

CWeaponComponent::EType CWeaponComponent::getWeaponTypeByName(const std::string& name) {
	CWeaponComponent::EType etype = EInvalid;
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
