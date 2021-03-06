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

CWeaponComponent::TWeaponInfo CWeaponComponent::s_aWeaponInfo[] = {
#define REG_WEAPON(val, name) \
	{CWeaponComponent::E##val, name},
#include "REG_WEAPONS.h"
#undef REG_WEAPON
};

CWeaponComponent::EType CWeaponComponent::getWeaponTypeByName(const std::string& name) {
	CWeaponComponent::EType etype = CWeaponComponent::EInvalid;
	int i = 0;
	while ((etype == CWeaponComponent::EInvalid) && (i < CWeaponComponent::NUM_WEAPON_TYPES)) {
		if (name == s_aWeaponInfo[i].sName) {
			etype = s_aWeaponInfo[i].eType;
		}
		i++;
	}
	return etype;
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
				CEntity* pBullet = g_pEntitiesFactory->createBullet(m_pOwner->getPos(), m_eType, v2bulletDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				float fAngle = vangle(m_v2AimDir);
				fAngle += DEG2RAD(s_fShotgunAngle);
				v2bulletDir = vunit(fAngle);
				pBullet = g_pEntitiesFactory->createBullet(m_pOwner->getPos(), m_eType, v2bulletDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				fAngle = vangle(m_v2AimDir);
				fAngle -= DEG2RAD(s_fShotgunAngle);
				v2bulletDir = vunit(fAngle);
				pBullet = g_pEntitiesFactory->createBullet(m_pOwner->getPos(), m_eType, v2bulletDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				break;
			}
			case EMINES: {
				CEntity* pBullet = g_pEntitiesFactory->createBullet(m_pOwner->getPos(), m_eType, m_v2AimDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				break;
			}
			case EC4: {
				m_pRemoteBullet = g_pEntitiesFactory->createBullet(m_pOwner->getPos(), m_eType, m_v2AimDir, m_pOwner->getType());
				m_pRemoteBullet->registerToDestroy(this);
				g_pWorld->addEntity(m_pRemoteBullet);
				break;
			}
			case EROCKETLAUNCHER: {
				CEntity* pBullet = g_pEntitiesFactory->createBullet(m_pOwner->getPos(), m_eType, m_v2AimDir, m_pOwner->getType());
				g_pWorld->addEntity(pBullet);
				break;
			}
			case ENUCLEARBOMB: {
				CEntity* pExplossion = g_pEntitiesFactory->createExplossion(m_pOwner->getPos(), m_eType);
				g_pWorld->addEntity(pExplossion);
				break;
			}
			default: {
				for (size_t i = 0; i < m_uBulletsPerShot; i++) {
					float fAngle = vangle(m_v2AimDir) + (i * 2.0f * M_PIf / m_uBulletsPerShot);
					CEntity* pBullet = g_pEntitiesFactory->createBullet(m_pOwner->getPos(), m_eType, vunit(fAngle), m_pOwner->getType());
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
		messageFire.bIsFiring = m_bIsFiring;
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
		m_bIsFiring = pMessage->bIsFiring;
	}
	else if (TMessageSetAimDirection* pMessage = dynamic_cast<TMessageSetAimDirection*>(_pMessage)) {
		m_v2AimDir = pMessage->v2Dir;
	}
	else if (TMessageGetAimDirection* pMessage = dynamic_cast<TMessageGetAimDirection*>(_pMessage)) {
		pMessage->v2Dir = m_v2AimDir;
	}
	else if (TMessageAmmoInfo* pMessage = dynamic_cast<TMessageAmmoInfo*>(_pMessage)) {
		pMessage->iCurrentAmmo = m_iCurrentBullets;
		pMessage->iTotalAmmo = m_iMaxBullets;
		pMessage->fReloadPercent = clamp((m_fReloadTime - m_fReloadTimer) / m_fReloadTime, 0.0f, 1.0f);
	}
	else if (TMessageReload* pMessage = dynamic_cast<TMessageReload*>(_pMessage)) {
		if (m_iCurrentBullets < m_iMaxBullets && m_fReloadTimer <= 0.0f) {
			m_bIsFiring = false;
			m_fReloadTimer = m_fReloadTime;
		}
	}
}

void CWeaponComponent::onEntityDestroyed(CEntity* _pEntity) {
	if (m_pRemoteBullet == _pEntity) {
		m_pRemoteBullet = nullptr;
		m_bIsFiring = false;
		m_fReloadTimer = m_fReloadTime;
	}
}
