#pragma once

#include "../../input/input_manager.h"

#define SHOTGUN_DISP_ANGLE 15.0f

class  Entity;
struct Message;
class  Sprite;
class  Text;

//=============================================================================
// Component class
//=============================================================================
class Component {
public:
	enum TWeapon {
		ERevolver,
		EMachinegun,
		EShotgun,
		EMines,
		EC4,
		ERocketLauncher,
		ENuclearBomb,
		EWeaponCount = 7
	};

	struct TWeaponData {
		TWeapon type;
		int     fireRate;
		int     reloadTime;
		int     capacity;
		int     bulletSpeed;
		int     bulletDamage;
		int     bulletLife;
		int     bulletRange;
		bool    isAutomatic;
		bool    isExplossive;
		bool    isBouncy;
	};

	enum TColliderType {
		ENoneCollider,
		ERectCollider,
		ECircleCollider,
	};

	virtual ~Component() {}

	virtual void init          ();
	virtual void destroy       ();
	virtual void activate      ();
	virtual void deactivate    ();
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message) {}

	void setActivationDelay(int activationDelay) { m_activationDelay = activationDelay; }
	//revisar esto de = 0 {} //Sirve para hacer la clase abstracta y a la vez no forzar la implementacion del destructor por los hijos
	//virtual ~Component() = 0 {}
	
	//void setOwner(Entity* owner) { m_owner = owner; }
protected:
	Component(Entity* owner) : m_owner(owner), m_isActive(false), m_activationDelay(0), m_activationTimer(0) {}

	Entity* m_owner;
	bool    m_isActive;
	int     m_activationDelay;
	int     m_activationTimer;
};

//=============================================================================
// ComponentTransform class
//=============================================================================
class ComponentTransform : public Component {
public:
	ComponentTransform(Entity* owner, const vec2& pos, const vec2& size)                           : Component(owner), m_pos(pos), m_size(size)                                 {}
	ComponentTransform(Entity* owner, const vec2& pos, const vec2& size, const vec2 sizeIncrement) : Component(owner), m_pos(pos), m_size(size), m_sizeIncrement(sizeIncrement) {}

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
	ComponentLife(Entity* owner, int life, int timeToLive, int invencibleTime);

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
	ComponentMove(Entity* owner, const vec2& direction, float speed, bool hasInertia, bool hasBounce) : Component(owner), m_direction(direction), m_speed(speed), m_hasInertia(hasInertia), m_hasBounce(hasBounce) {}
	
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
	ComponentRenderable(Entity* owner, const char* texture, float angle, float alpha, int priority, int hitTime = 0);
	~ComponentRenderable();

	virtual void init          ();
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	Sprite*     m_sprite;
	const char* m_texture;
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
	ComponentWeapon(Entity* owner, TWeaponData weaponData);

	virtual void init          ();
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	TWeaponData m_weaponData;
	Entity*            m_remoteBullet;
	vec2               m_aimDirection;
	int                m_currentBullets;
	bool               m_isFiring;

	//Timers
	int m_fireTimer;
	int m_reloadTimer;
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
private:
	Entity* m_player;
	float   m_speed;
	float   m_maxDistance;
	vec2    m_offset;
};

//=============================================================================
// ComponentAIEvade class
//=============================================================================
class ComponentAIEvade: public Component {
public:
	ComponentAIEvade(Entity* owner, Entity* player, float speed, float range) : Component(owner), m_player(player), m_speed(speed), m_range(range) {}
	
	virtual void run(float deltaTime);
private:
	vec2 calculatIntersectionWithWall(vec2 position, float angle);

	Entity* m_player;
	float   m_speed;
	float   m_range;
};

//=============================================================================
// ComponentAIFire class
//=============================================================================
class ComponentAIFire : public Component {
public:
	ComponentAIFire(Entity* owner, Entity* player)            : Component(owner), m_player(player) {}
	ComponentAIFire(Entity* owner, std::vector<vec2> fireDirections, bool shuffle) : Component(owner), m_fireDirections(fireDirections), m_shuffle(shuffle) {}
	
	virtual void init          ();
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	Entity*           m_player;
	std::vector<vec2> m_fireDirections;
	int               m_currentFireDirection;
	bool              m_shuffle;
};

//=============================================================================
// ComponentCollider class
//=============================================================================
class ComponentCollider : public Component {
public:
	enum TColliderChannel {
		EPlayer       = 1 << 0,
		EEnemyC        = 1 << 1,
		EPlayerWeapon = 1 << 2,
		EEnemyWeapon  = 1 << 3,
		EBoundaries   = 1 << 4,
		EPickup       = 1 << 5,
		ENone         = 0,
	};

	ComponentCollider(Entity* owner, TColliderType type, int deltaLife, int collisionChannel, int collisionChannelsResponse) : Component(owner), m_type(type), m_deltaLife(deltaLife), m_collisionChannel(collisionChannel), m_collisionChannelsResponse(collisionChannelsResponse) {}
	
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	TColliderType m_type;
	int m_collisionChannel;
	int m_collisionChannelsResponse;
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
	ComponentWeaponPickup(Entity* owner, TWeapon weapon) : Component(owner), m_weapon(weapon) {}
	
	virtual void receiveMessage(Message* message);
private:
	TWeapon m_weapon;
};

//=============================================================================
// ComponentHUDMessage class
//=============================================================================
class ComponentHUDMessage : public Component {
public:
	ComponentHUDMessage(Entity* owner, vec2 pos, std::string messageText) : Component(owner), m_pos(pos), m_messageText(messageText), m_message(nullptr) {}
	~ComponentHUDMessage();

	virtual void init();
private:
	vec2        m_pos;
	std::string m_messageText;
	Text*       m_message;
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
	Text*   m_life;
	Text*   m_score;
	Text*   m_ammo;
	Text*   m_fps;
	Sprite* m_target;

	//ver si cambiar a Renderable
	Sprite* m_reloadAnim;
};
