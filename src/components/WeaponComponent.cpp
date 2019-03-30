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

CWeaponComponent::~CWeaponComponent() {
	if (m_pRemoteBullet) {
		m_pRemoteBullet->unregisterToDestroy(this);
	}
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
	CComponent::init();
	
	equipWeapon(m_eType);
}

void CWeaponComponent::run(float _fDeltaTime) {
	CComponent::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	ASSERT(m_pOwner && g_pWorld && g_pEntitiesFactory && g_pSoundEngine);

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
	}
	else if (m_bIsFiring && m_iFireTimer <= 0 && m_iReloadTimer <= 0 && m_iCurrentBullets != 0) {
		switch (m_eType) {
			case ESHOTGUN: {
				vec2 v2bulletDir = m_v2AimDir;
				Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), v2bulletDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				float fAngle = vangle(m_v2AimDir);
				fAngle += s_fShotgunAngle;
				v2bulletDir = vunit(DEG2RAD(fAngle));
				pBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), v2bulletDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				fAngle = vangle(m_v2AimDir);
				fAngle -= s_fShotgunAngle;
				v2bulletDir = vunit(DEG2RAD(fAngle));
				pBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), v2bulletDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				break;
			}
			case EMINES: {
				Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), m_v2AimDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				break;
			}
			case EC4: {
				m_pRemoteBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), m_v2AimDir, m_pOwner->getType());
				m_pRemoteBullet->registerToDestroy(this);
				g_pWorld->addEntity(m_pRemoteBullet);
				break;
			}
			case EROCKETLAUNCHER: {
				Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), m_v2AimDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				break;
			}
			case ENUCLEARBOMB: {
				Entity* pExplossion = g_pEntitiesFactory->createExplossion(m_pOwner->getPos(), m_eType);
				g_pWorld->addEntity(pExplossion);
				break;
			}
			default: {
				for (size_t i = 0; i < m_uBulletsPerShot; i++) {
					float fAngle = vangle(m_v2AimDir) + (i * 360.f / m_uBulletsPerShot);
					Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), vunit(DEG2RAD(fAngle)), m_pOwner->getType());
					g_pWorld->addEntity(pBullet);
				}
				break;
			}
		}

		m_iFireTimer = m_iFireRate;
		--m_iCurrentBullets;
		if (m_iCurrentBullets == 0 && !m_pRemoteBullet) {
			m_iReloadTimer = m_iReloadTime;
		}
		if (!m_bIsAutomatic) {
			m_bIsFiring = false;
		}

		if (m_sSoundFile != "") {
			g_pSoundEngine->playSound(m_sSoundFile.c_str());
		}

		MessageFire messageFire;
		messageFire.isFiring = m_bIsFiring;
		m_pOwner->receiveMessage(&messageFire);
	}
}

void CWeaponComponent::receiveMessage(Message* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage);

	if (MessageWeaponChange* pMessage = dynamic_cast<MessageWeaponChange*>(_pMessage)) {
		equipWeapon(pMessage->eWeaponType);
	}
	else if (MessageFire* pMessage = dynamic_cast<MessageFire*>(_pMessage)) {
		m_bIsFiring = pMessage->isFiring;
	}
	else if (MessageSetAimDirection* pMessage = dynamic_cast<MessageSetAimDirection*>(_pMessage)) {
		m_v2AimDir = pMessage->direction;
	}
	else if (MessageGetAimDirection* pMessage = dynamic_cast<MessageGetAimDirection*>(_pMessage)) {
		pMessage->direction = m_v2AimDir;
	}
	else if (MessageAmmoInfo* pMessage = dynamic_cast<MessageAmmoInfo*>(_pMessage)) {
		pMessage->currentAmmo = m_iCurrentBullets;
		pMessage->totalAmmo = m_iMaxBullets;
		pMessage->reloadPercent = clamp((m_iReloadTime - m_iReloadTimer) * 1.0f / m_iReloadTime, 0.0f, 1.0f);
	}
	else if (MessageReload* pMessage = dynamic_cast<MessageReload*>(_pMessage)) {
		if (m_iCurrentBullets < m_iMaxBullets && m_iReloadTimer <= 0) {
			m_bIsFiring = false;
			m_iReloadTimer = m_iReloadTime;
		}
	}
}

void CWeaponComponent::onEntityDestroyed(Entity* _pEntity) {
	if (m_pRemoteBullet == _pEntity) {
		m_pRemoteBullet = nullptr;
		m_bIsFiring = false;
		m_iReloadTimer = m_iReloadTime;
	}
}
