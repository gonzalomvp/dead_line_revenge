#include "common/stdafx.h"
#include "WeaponComponent.h"

#include "engine/SoundEngine.h"
#include "entities/Entity.h"
#include "entities/EntitiesFactory.h"
#include "messages/Message.h"
#include "scenes/World.h"

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
	m_fFireRate = weaponData.fFireRate;
	m_fFireTimer = 0.0f;
	m_fReloadTime = weaponData.fReloadTime;
	m_fReloadTimer = 0.0f;
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

	if (m_fFireTimer > 0.0f) {
		m_fFireTimer -= _fDeltaTime;
	}

	if (m_fReloadTimer > 0.0f) {
		m_fReloadTimer -= _fDeltaTime;
		if (m_fReloadTimer <= 0) {
			m_iCurrentBullets = m_iMaxBullets;
			m_bIsFiring = false;
		}
	}

	if (m_bIsFiring && m_pRemoteBullet) {
		TMessageDestroy msgDestroy;
		m_pRemoteBullet->receiveMessage(&msgDestroy);
	}
	else if (m_bIsFiring && m_fFireTimer <= 0.0f && m_fReloadTimer <= 0.0f && m_iCurrentBullets != 0) {
		switch (m_eType) {
			case ESHOTGUN: {
				vec2 v2bulletDir = m_v2AimDir;
				Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), v2bulletDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				float fAngle = RAD2DEG(vangle(m_v2AimDir));
				fAngle += s_fShotgunAngle;
				v2bulletDir = vunit(DEG2RAD(fAngle));
				pBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), v2bulletDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				fAngle = RAD2DEG(vangle(m_v2AimDir));
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
					float fAngle = RAD2DEG(vangle(m_v2AimDir) + (i * 360.f / m_uBulletsPerShot));
					Entity* pBullet = g_pEntitiesFactory->createBullet(m_eType, m_pOwner->getPos(), vunit(DEG2RAD(fAngle)), m_pOwner->getType());
					g_pWorld->addEntity(pBullet);
				}
				break;
			}
		}

		m_fFireTimer = m_fFireRate;
		--m_iCurrentBullets;
		if (m_iCurrentBullets == 0 && !m_pRemoteBullet) {
			m_fReloadTimer = m_fReloadTime;
		}
		if (!m_bIsAutomatic) {
			m_bIsFiring = false;
		}

		if (m_sSoundFile != "") {
			g_pSoundEngine->playSound(m_sSoundFile.c_str());
		}

		TMessageFire messageFire;
		messageFire.isFiring = m_bIsFiring;
		m_pOwner->receiveMessage(&messageFire);
	}
}

void CWeaponComponent::receiveMessage(TMessage* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage);

	if (TMessageWeaponChange* pMessage = dynamic_cast<TMessageWeaponChange*>(_pMessage)) {
		equipWeapon(pMessage->eWeaponType);
	}
	else if (TMessageFire* pMessage = dynamic_cast<TMessageFire*>(_pMessage)) {
		m_bIsFiring = pMessage->isFiring;
	}
	else if (TMessageSetAimDirection* pMessage = dynamic_cast<TMessageSetAimDirection*>(_pMessage)) {
		m_v2AimDir = pMessage->direction;
	}
	else if (TMessageGetAimDirection* pMessage = dynamic_cast<TMessageGetAimDirection*>(_pMessage)) {
		pMessage->direction = m_v2AimDir;
	}
	else if (TMessageAmmoInfo* pMessage = dynamic_cast<TMessageAmmoInfo*>(_pMessage)) {
		pMessage->currentAmmo = m_iCurrentBullets;
		pMessage->totalAmmo = m_iMaxBullets;
		pMessage->reloadPercent = clamp((m_fReloadTime - m_fReloadTimer) / m_fReloadTime, 0.0f, 1.0f);
	}
	else if (TMessageReload* pMessage = dynamic_cast<TMessageReload*>(_pMessage)) {
		if (m_iCurrentBullets < m_iMaxBullets && m_fReloadTimer <= 0.0f) {
			m_bIsFiring = false;
			m_fReloadTimer = m_fReloadTime;
		}
	}
}

void CWeaponComponent::onEntityDestroyed(Entity* _pEntity) {
	if (m_pRemoteBullet == _pEntity) {
		m_pRemoteBullet = nullptr;
		m_bIsFiring = false;
		m_fReloadTimer = m_fReloadTime;
	}
}
