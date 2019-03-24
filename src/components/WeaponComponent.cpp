#include "common/stdafx.h"
#include "WeaponComponent.h"

#include "engine/sound_engine.h"
#include "entities/entity.h"
#include "entities/entities_factory.h"
#include "messages/message.h"
#include "scenes/world.h"

namespace {
	const float s_fShotgunAngle = 15.0f;
}

void CWeaponComponent::equipWeapon(EType _eType) {
	ASSERT(g_pEntitiesFactory);

	CEntitiesFactory::TWeaponDef weaponData = g_pEntitiesFactory->getWeaponDef(_eType);
	m_eType = _eType;
	m_iMaxBullets = weaponData.iMaxBullets;
	m_iCurrentBullets = weaponData.iMaxBullets;
	m_uBulletsPerShot = weaponData.uBulletsPerShot;
	m_iFireRate = weaponData.iFireRate;
	m_iFireTimer = 0;
	m_iReloadTime = weaponData.iReloadTime;
	m_iReloadTimer = 0;
	m_bIsAutomatic = weaponData.bIsAutomatic;
	m_sSoundFile = weaponData.sSoundFile;
}

void CWeaponComponent::init() {
	Component::init();
	
	equipWeapon(m_eType);
}

void CWeaponComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	ASSERT(m_owner && g_pWorld && g_pEntitiesFactory && g_pSoundEngine);

	if (m_iFireTimer > 0) {
		--m_iFireTimer;
	}

	if (m_iReloadTimer > 0) {
		--m_iReloadTimer;
		if (m_iReloadTimer <= 0) {
			m_iCurrentBullets = m_iMaxBullets;
			m_bIsFiring = false;
		}
	}

	if (m_bIsFiring && m_pRemoteBullet) {
		MessageDestroy msgDestroy;
		m_pRemoteBullet->receiveMessage(&msgDestroy);
		m_pRemoteBullet = nullptr;
		m_bIsFiring = false;
		m_iCurrentBullets = m_iMaxBullets;
	}
	else if (m_bIsFiring && m_iFireTimer <= 0 && m_iReloadTimer <= 0 && m_iCurrentBullets > 0) {
		m_iFireTimer = m_iFireRate;
		--m_iCurrentBullets;
		if (m_iCurrentBullets == 0) {
			m_iReloadTimer = m_iReloadTime;
		}
		if (!m_bIsAutomatic) {
			m_bIsFiring = false;
		}

		MessageGetTransform messageGetTranform;
		m_owner->receiveMessage(&messageGetTranform);

		switch (m_eType) {
			case ESHOTGUN: {
				vec2 v2bulletDir = m_v2AimDirection;
				Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, v2bulletDir, m_owner->getType());
				g_pWorld->addEntity(pBullet);
				float fAngle = vangle(m_v2AimDirection);
				fAngle += s_fShotgunAngle;
				v2bulletDir = vunit(DEG2RAD(fAngle));
				pBullet = g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, v2bulletDir, m_owner->getType());
				g_pWorld->addEntity(pBullet);
				fAngle = vangle(m_v2AimDirection);
				fAngle -= s_fShotgunAngle;
				v2bulletDir = vunit(DEG2RAD(fAngle));
				pBullet = g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, v2bulletDir, m_owner->getType());
				g_pWorld->addEntity(pBullet);
				break;
			}
			case EMINES: {
				Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, m_v2AimDirection, m_owner->getType());
				g_pWorld->addEntity(pBullet);
				break;
			}
			case EC4: {
				m_pRemoteBullet = g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, m_v2AimDirection, m_owner->getType());
				g_pWorld->addEntity(m_pRemoteBullet);
				break;
			}
			case EROCKETLAUNCHER: {
				Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, m_v2AimDirection, m_owner->getType());
				g_pWorld->addEntity(pBullet);
				break;
			}
			case ENUCLEARBOMB: {
				Entity* pExplossion = g_pEntitiesFactory->createExplossion(messageGetTranform.pos, vmake(20.0f, 20.0f), vmake(8.0f, 8.0f), 100, Entity::ENUCLEAREXPLOSSION);
				g_pWorld->addEntity(pExplossion);
				break;
			}
			default: {
				for (size_t i = 0; i < m_uBulletsPerShot; i++) {
					float fAngle = vangle(m_v2AimDirection) + (i * 360.f / m_uBulletsPerShot);
					Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, messageGetTranform.pos, vunit(DEG2RAD(fAngle)), m_owner->getType());
					g_pWorld->addEntity(pBullet);
				}
				break;
			}
		}

		if (m_sSoundFile != "") {
			g_pSoundEngine->playSound(m_sSoundFile.c_str());
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
		equipWeapon(msgWeaponChange->eWeaponType);
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
				MessageGetAimDirection* messageGetAimDirection = dynamic_cast<MessageGetAimDirection*>(message);
				if (messageGetAimDirection) {
					messageGetAimDirection->direction = m_v2AimDirection;
				}
				else {
					MessageAmmoInfo* msgAmmoInfo = dynamic_cast<MessageAmmoInfo*>(message);
					if (msgAmmoInfo) {
						msgAmmoInfo->currentAmmo = m_iCurrentBullets;
						msgAmmoInfo->totalAmmo = m_iMaxBullets;
						msgAmmoInfo->reloadPercent = clamp((m_iReloadTime - m_iReloadTimer) * 1.0f / m_iReloadTime, 0.0f, 1.0f);
					}
					else {
						MessageReload* msgReload = dynamic_cast<MessageReload*>(message);
						if (msgReload && m_iCurrentBullets < m_iMaxBullets && m_iReloadTimer <= 0) {
							m_bIsFiring = false;
							m_iReloadTimer = m_iReloadTime;
						}
					}
				}
			}
		}
	}
}
