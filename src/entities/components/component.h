#pragma once

#include "../../input/input_manager.h"

#define SHOTGUN_DISP_ANGLE 15.0f

class  Entity;
class  GfxEntity;
struct Message;
class  Sprite;
class  Text;

//=============================================================================
// Component class
//=============================================================================
class Component {
public:
	virtual ~Component() {}

	virtual void init          ();
	virtual void activate      ();
	virtual void deactivate    ();
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message) {}

	void setActivationDelay(int activationDelay) { m_activationDelay = activationDelay; }
protected:
	Component(Entity* owner) : m_owner(owner), m_isActive(false), m_activationDelay(0), m_activationTimer(0) {}

	Entity* m_owner;
	bool        m_isActive;
	int         m_activationDelay;
	int         m_activationTimer;
};

//=============================================================================
// ComponentTransform class
//=============================================================================
class ComponentTransform : public Component {
public:
	ComponentTransform(Entity* owner, const vec2& pos, const vec2& size, const vec2& sizeIncrement = vmake(0.0f, 0.0f)) :
		Component(owner),
		m_pos(pos),
		m_size(size),
		m_sizeIncrement(sizeIncrement) {}

	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	vec2 m_pos;
	vec2 m_size;
	vec2 m_sizeIncrement;
};

//=============================================================================
// ComponentLife class
//=============================================================================
class ComponentLife: public Component {
public:
	ComponentLife(Entity* owner, int life, int timeToLive, int invencibleTime) :
		Component(owner), 
		m_life(life), 
		m_timeToLive(timeToLive), 
		m_invencibleTime(invencibleTime), 
		m_lifeTimer(0), 
		m_invencibleTimer(0) {}

	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	int m_life;
	int m_timeToLive;
	int m_invencibleTime;

	//Timers
	int m_lifeTimer;
	int m_invencibleTimer;
};

//=============================================================================
// ComponentMove class
//=============================================================================
class ComponentMove : public Component {
public:
	ComponentMove(Entity* owner, const vec2& direction, float speed, bool hasInertia, bool hasBounce) :
		Component(owner), 
		m_direction(direction),
		m_speed(speed),
		m_hasInertia(hasInertia), 
		m_hasBounce(hasBounce) {}
	
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	vec2  m_direction;
	float m_speed;
	bool  m_hasInertia;
	bool  m_hasBounce;
};

//=============================================================================
// ComponentRenderable class
//=============================================================================
class ComponentRenderable : public Component {
public:
	ComponentRenderable(Entity* owner, const std::string& texture, float angle, float alpha, int priority, int hitTime = 0) :
		Component(owner),
		m_texture(texture),
		m_angle(angle),
		m_alpha(alpha),
		m_priority(priority),
		m_hitTime(hitTime),
		m_hitTimer(hitTime) {}
	~ComponentRenderable();

	virtual void init          ();
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	Sprite* m_sprite;
	std::string m_texture;
	int         m_priority;
	float       m_alpha;
	float       m_angle;
	int         m_hitTime;

	//Timers
	int     m_hitTimer;
};

//=============================================================================
// ComponentPlayerController class
//=============================================================================
class ComponentPlayerController : public Component, public IInputManager::IListener {
public:
	ComponentPlayerController(Entity* owner) : Component(owner) {}
	~ComponentPlayerController();

	virtual void init   ();
	virtual bool onEvent(const IInputManager::Event&);
};

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

	enum TType {
#define REG_WEAPON(val, name) \
		EE##val,
#include "REG_WEAPONS.h"
#undef REG_WEAPON

		EInvalid,
	};

	static const int NUM_WEAPON_TYPES = EInvalid;
	static const int NUM_PLAYER_WEAPONS_TYPES = EENUM_PLAYER_WEAPONS;

	struct TWeaponData {
		TType       type;
		int         fireRate;
		int         reloadTime;
		int         capacity;
		float       bulletSpeed;
		int         bulletDamage;
		int         bulletLife;
		int         bulletRange;
		int         numBullets;
		bool        isAutomatic;
		bool        isExplossive;
		bool        isBouncy;
		std::string soundFile;
	};

	ComponentWeapon(Entity* owner, TWeaponData weaponData) : 
		Component(owner),
		m_weaponData(weaponData),
		m_remoteBullet(nullptr),
		m_aimDirection(vmake(0.0f, 0.0f)),
		m_isFiring(false),
		m_currentBullets(m_weaponData.capacity),
		m_fireTimer(m_weaponData.fireRate),
		m_reloadTimer(m_weaponData.reloadTime) {}

	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);

	static TType getWeaponTypeByName(const std::string& name);
private:
	TWeaponData m_weaponData;
	Entity* m_remoteBullet;
	vec2        m_aimDirection;
	int         m_currentBullets;
	bool        m_isFiring;

	//Timers
	int m_fireTimer;
	int m_reloadTimer;

	struct TWeaponInfo
	{
		TType eType;
		std::string sName;
	};
	static TWeaponInfo s_aWeaponInfo[];
};

//=============================================================================
// ComponentExplossive class
//=============================================================================
class ComponentExplossive : public Component {
public:
	ComponentExplossive(Entity* owner) : Component(owner) {}

	virtual void receiveMessage(Message* message);
};

//=============================================================================
// ComponentAIMelee class
//=============================================================================
class ComponentAIMelee : public Component {
public:
	ComponentAIMelee(Entity* owner, Entity* player, float speed, float maxDistance);
	
	virtual void run(float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	Entity* m_player;
	float       m_speed;
	float       m_maxDistance;
	vec2        m_offset;
};

//=============================================================================
// ComponentAIEvade class
//=============================================================================
class ComponentAIEvade: public Component {
public:
	ComponentAIEvade(Entity* owner, Entity* player, float speed, float range) : Component(owner), m_player(player), m_speed(speed), m_range(range) {}
	
	virtual void run(float deltaTime);
private:
	vec2 calculatIntersectionWithWall(const vec2& position, float angle);

	Entity* m_player;
	float       m_speed;
	float       m_range;
};

//=============================================================================
// ComponentAIFire class
//=============================================================================
class ComponentAIFire : public Component {
public:
	ComponentAIFire(Entity* owner, Entity* player) : Component(owner), m_player(player) {}
	ComponentAIFire(Entity* owner, const std::vector<vec2>& fireDirections, bool shuffle) : 
		Component(owner), 
		m_fireDirections(fireDirections),
		m_currentFireDirection(0),
		m_shuffle(shuffle) {}
	
	virtual void init          ();
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	Entity*       m_player;
	std::vector<vec2> m_fireDirections;
	size_t            m_currentFireDirection;
	bool              m_shuffle;
};

//=============================================================================
// ComponentCollider class
//=============================================================================
class ComponentCollider : public Component {
public:
	enum TColliderType {
		ERectCollider,
		ECircleCollider,
	};

	enum TColliderChannel {
		EPlayerCollider       = 1 << 0,
		EEnemyCollider        = 1 << 1,
		EPlayerWeaponCollider = 1 << 2,
		EEnemyWeaponCollider  = 1 << 3,
		EBoundariesCollider   = 1 << 4,
		EPickupCollider       = 1 << 5,
		ENoneCollider         = 0,
	};

	ComponentCollider(Entity* owner, TColliderType type, int deltaLife, int collisionChannel, int collisionChannelsResponse) :
		Component(owner),
		m_type(type),
		m_center(vmake(0.0f, 0.0f)),
		m_size(vmake(0.0f, 0.0f)),
		m_deltaLife(deltaLife), 
		m_collisionChannel(collisionChannel),
		m_collisionChannelsResponse(collisionChannelsResponse) {}
	
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	TColliderType m_type;
	int           m_collisionChannel;
	int           m_collisionChannelsResponse;
	vec2          m_center;
	vec2          m_size;
	int           m_deltaLife;
};

//=============================================================================
// ComponentPoints class
//=============================================================================
class ComponentPoints : public Component {
public:
	ComponentPoints(Entity* owner, int points): Component(owner), m_points(points) {}

	virtual void receiveMessage(Message* message);
private:
	int m_points;
};

//=============================================================================
// ComponentWeaponPickup class
//=============================================================================
class ComponentWeaponPickup : public Component {
public:
	ComponentWeaponPickup(Entity* owner, ComponentWeapon::TWeaponData weaponData) : Component(owner), m_weaponData(weaponData) {}
	
	virtual void receiveMessage(Message* message);
private:
	ComponentWeapon::TWeaponData m_weaponData;
};

//=============================================================================
// ComponentHUDMessage class
//=============================================================================
class ComponentHUDMessage : public Component {
public:
	ComponentHUDMessage(Entity* owner, vec2 pos, const std::string& messageText) : 
		Component(owner), m_pos(pos),
		m_messageText(messageText),
		m_message(nullptr) {}
	~ComponentHUDMessage();

	virtual void init();
private:
	vec2        m_pos;
	std::string m_messageText;
	Text*   m_message;
};

//=============================================================================
// ComponentHUD class
//=============================================================================
class ComponentHUD : public Component, public IInputManager::IListener {
public:
	ComponentHUD(Entity* owner) : Component(owner) {}
	~ComponentHUD();

	virtual void init   ();
	virtual void run    (float deltaTime);
	virtual bool onEvent(const IInputManager::Event&);
private:
	Text*                   m_life;
	Text*                   m_score;
	Text*                   m_ammo;
	Sprite*                 m_target;
	Sprite*                 m_reloadAnim;
	std::vector<GfxEntity*> m_gfxEntities;
};

//=============================================================================
// Collision check utils
//=============================================================================
inline float clamp(float value, float inf, float sup) { return (value <= inf) ? inf : (value >= sup) ? sup : value; };
inline bool  isInBounds(float value, float inf, float sup) { return (value > inf) && (value < sup); };
bool         checkCircleCircle(const vec2& pos1, float radius1, const vec2& pos2, float radius2);
bool         checkCircleRect(const vec2& circlePos, float circleRadius, const vec2& rectPos, const vec2& rectSize);
bool         checkRectRect(const vec2& rectPos1, const vec2& rectSize1, const vec2& rectPos2, const vec2& rectSize2);