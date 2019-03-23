#include "common/stdafx.h"
#include "WeaponComponent.h"

#include "engine/sound_engine.h"
#include "entities/entity.h"
#include "entities/entities_factory.h"
#include "messages/message.h"
#include "scenes/world.h"

#define SHOTGUN_DISP_ANGLE 15.0f

void CWeaponComponent::init() {
	Component::init();

	ASSERT(g_pEntitiesFactory);
	CEntitiesFactory::TWeaponDef weaponData = g_pEntitiesFactory->getWeaponDef(m_eType);
	m_iCurrentBullets = weaponData.iCapacity;
	m_iFireTimer = weaponData.iFireRate;
	m_iReloadTimer = weaponData.iReloadTime;
}

void CWeaponComponent::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	ASSERT(g_pEntitiesFactory);

	CEntitiesFactory::TWeaponDef weaponData = g_pEntitiesFactory->getWeaponDef(m_eType);
	if (m_iFireTimer < weaponData.iFireRate) {
		++m_iFireTimer;
	}
	if (m_iReloadTimer <= weaponData.iReloadTime) {
		++m_iReloadTimer;
		if (m_iReloadTimer == weaponData.iReloadTime) {
			m_iCurrentBullets = weaponData.iCapacity;
			m_bIsFiring = false;
		}
	}

	if (m_bIsFiring && m_pRemoteBullet) {
		MessageDestroy msgDestroy;
		m_pRemoteBullet->receiveMessage(&msgDestroy);
		m_pRemoteBullet = nullptr;
		m_bIsFiring = false;
		m_iCurrentBullets = weaponData.iCapacity;
	}

	else if (m_bIsFiring && m_iFireTimer >= weaponData.iFireRate && m_iReloadTimer >= weaponData.iReloadTime && m_iCurrentBullets > 0) {
		m_iFireTimer = 0;
		--m_iCurrentBullets;
		if (m_iCurrentBullets == 0) {
			m_iReloadTimer = 0;
		}
		if (!weaponData.bIsAutomatic) {
			m_bIsFiring = false;
		}

		MessageGetTransform messageGetTranform;
		m_owner->receiveMessage(&messageGetTranform);

		switch (weaponData.eType) {
		case ESHOTGUN: {
			vec2 bulletDir = m_v2AimDirection;
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, bulletDir, m_owner->getType()));
			float angle = vangle(m_v2AimDirection);
			angle += SHOTGUN_DISP_ANGLE;
			bulletDir = vunit(DEG2RAD(angle));
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, bulletDir, m_owner->getType()));
			angle = vangle(m_v2AimDirection);
			angle -= SHOTGUN_DISP_ANGLE;
			bulletDir = vunit(DEG2RAD(angle));
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, bulletDir, m_owner->getType()));
			break;
		}
		case EMINES: {
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, m_v2AimDirection, m_owner->getType()));
			break;
		}
		case EC4: {
			m_pRemoteBullet = g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, m_v2AimDirection, m_owner->getType());
			g_pWorld->addEntity(m_pRemoteBullet);
			break;
		}
		case EROCKETLAUNCHER: {
			g_pWorld->addEntity(g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, m_v2AimDirection, m_owner->getType()));
			break;
		}
		case ENUCLEARBOMB: {
			g_pWorld->addEntity(g_pEntitiesFactory->createExplossion(messageGetTranform.pos, vmake(20.0f, 20.0f), vmake(8.0f, 8.0f), 100, Entity::ENUCLEAREXPLOSSION));
			break;
		}
		default: {
			if (weaponData.uNumBullets > 1) {
				for (size_t i = 0; i < weaponData.uNumBullets; i++) {
					float angle = vangle(m_v2AimDirection) + (i * 360.f / weaponData.uNumBullets);
					g_pWorld->addEntity(g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, vunit(DEG2RAD(angle)), m_owner->getType()));
				}
			}
			else {
				g_pWorld->addEntity(g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, m_v2AimDirection, m_owner->getType()));
			}

			break;
		}
		}

		if (weaponData.sSoundFile != "") {
			g_pSoundEngine->playSound(weaponData.sSoundFile.c_str());
		}

		MessageFire messageFire;
		messageFire.isFiring = m_bIsFiring;
		messageFire.isFireDone = true;
		m_owner->receiveMessage(&messageFire);
	}
}

void CWeaponComponent::receiveMessage(Message* message) {
	if (!m_isActive)
		return;

	MessageWeaponChange* msgWeaponChange = dynamic_cast<MessageWeaponChange*>(message);
	if (msgWeaponChange) {
		CEntitiesFactory::TWeaponDef weaponData = g_pEntitiesFactory->getWeaponDef(msgWeaponChange->eWeaponType);
		m_eType = msgWeaponChange->eWeaponType;
		m_bIsFiring = false;
		m_iCurrentBullets = weaponData.iCapacity;
		m_iFireTimer = weaponData.iFireRate;
		m_iReloadTimer = weaponData.iReloadTime;
	}
	else {
		MessageFire* msgFire = dynamic_cast<MessageFire*>(message);
		if (msgFire) {
			m_bIsFiring = msgFire->isFiring;
		}
		else {
			MessageSetAimDirection* messageSetAimDirection = dynamic_cast<MessageSetAimDirection*>(message);
			if (messageSetAimDirection) {
				m_v2AimDirection = messageSetAimDirection->direction;
			}
			else {
				CEntitiesFactory::TWeaponDef weaponData = g_pEntitiesFactory->getWeaponDef(m_eType);
				MessageGetAimDirection* messageGetAimDirection = dynamic_cast<MessageGetAimDirection*>(message);
				if (messageGetAimDirection) {
					messageGetAimDirection->direction = m_v2AimDirection;
				}
				else {
					MessageAmmoInfo* msgAmmoInfo = dynamic_cast<MessageAmmoInfo*>(message);
					if (msgAmmoInfo) {
						msgAmmoInfo->currentAmmo = m_iCurrentBullets;
						msgAmmoInfo->totalAmmo = weaponData.iCapacity;
						msgAmmoInfo->reloadPercent = m_iReloadTimer * 1.0f / weaponData.iReloadTime;
						if (msgAmmoInfo->reloadPercent > 1.0f) {
							msgAmmoInfo->reloadPercent = 1.0f;
						}
					}
					else {
						MessageReload* msgReload = dynamic_cast<MessageReload*>(message);
						if (msgReload && m_iCurrentBullets < weaponData.iCapacity && m_iReloadTimer >= weaponData.iReloadTime) {
							m_bIsFiring = false;
							m_iReloadTimer = 0;
						}
					}
				}
			}
		}
	}
}
