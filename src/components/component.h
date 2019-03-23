#pragma once

#include "input/input_manager.h"

#define SHOTGUN_DISP_ANGLE 15.0f

class  Entity;
class  CGfxEntity;
struct Message;
class  CSprite;
class  CText;

//=============================================================================
// Component class
//=============================================================================
class Component {
public:
	Component(Entity* owner) : m_owner(owner), m_isActive(false), m_activationDelay(0), m_activationTimer(0) {}
	virtual ~Component() = 0 {};

	virtual void init          ();
	virtual void activate      ();
	virtual void deactivate    ();
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message) {}

	void setActivationDelay(int activationDelay) { m_activationDelay = activationDelay; }
	Entity* getOwner() { return m_owner; }

protected:
	Entity* m_owner;
	bool        m_isActive;
	int         m_activationDelay;
	int         m_activationTimer;
};

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
class ComponentWeapon : public Component {
public:
	//enum TWeapon {
	//	ERevolver,
	//	EMachinegun,
	//	EShotgun,
	//	EMines,
	//	EC4,
	//	ERocketLauncher,
	//	ENuclearBomb,
	//	EEnemyWeapon,
	//	EBossWeapon1,
	//	EBossWeapon2,
	//	ENone,
	//	EWeaponCount = 7
	//};

	enum EType {
#define REG_WEAPON(val, name) \
		E##val,
#include "REG_WEAPONS.h"
#undef REG_WEAPON

		EInvalid,
	};

	static const int NUM_WEAPON_TYPES = EInvalid;

	struct TWeaponData {
		EType       type;
		int         fireRate;
		int         reloadTime;
		int         capacity;
		float       bulletSpeed;
		int         bulletDamage;
		int         bulletLife;
		int         bulletRange;
		uint16_t    numBullets;
		bool        isAutomatic;
		bool        isExplossive;
		bool        isBouncy;
		std::string soundFile;
	};

	ComponentWeapon(Entity* owner, TWeaponData weaponData) : 
		Component(owner),
		m_mWeaponData(weaponData),
		m_remoteBullet(nullptr),
		m_aimDirection(vmake(0.0f, 0.0f)),
		m_isFiring(false),
		m_currentBullets(m_mWeaponData.capacity),
		m_fireTimer(m_mWeaponData.fireRate),
		m_reloadTimer(m_mWeaponData.reloadTime) {}

	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);

	static EType getWeaponTypeByName(const std::string& name);
private:
	TWeaponData m_mWeaponData;
	Entity* m_remoteBullet;
	vec2        m_aimDirection;
	int         m_currentBullets;
	bool        m_isFiring;

	//Timers
	int m_fireTimer;
	int m_reloadTimer;

	struct TWeaponInfo
	{
		EType eType;
		std::string sName;
	};
	static TWeaponInfo s_aWeaponInfo[];
};

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
