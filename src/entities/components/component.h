#pragma once

#include "../../input/input_manager.h"

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
		EWeaponCount = 6
	};

	enum TColliderType {
		ENoneCollider,
		ERectCollider,
		ECircleCollider,
	};

	enum TFaction {
		EAllied,
		EEnemy,
		ENeutral,
		EBounds,
	};

	virtual ~Component() {}

	virtual void init          ();
	virtual void destroy       ();
	virtual void activate      ();
	virtual void deactivate    ();
	virtual void run           ()                 {}
	virtual void receiveMessage(Message* message) {}
	//revisar esto de = 0 {} //Sirve para hacer la clase abstracta y a la vez no forzar la implementacion del destructor por los hijos
	//virtual ~Component() = 0 {}
	
	//void setOwner(Entity* owner) { m_owner = owner; }
protected:
	Component(Entity* owner);

	Entity* m_owner;
	bool    m_isActive;
};

//=============================================================================
// ComponentTransform class
//=============================================================================
class ComponentTransform : public Component {
public:
	ComponentTransform(Entity* owner, const vec2& pos, const vec2& size, const vec2 sizeDelta = vmake(0,0)) : Component(owner), m_pos(pos), m_size(size), m_sizeDelta(sizeDelta) {}

	virtual void run           ();
	virtual void receiveMessage(Message* message);
private:
	vec2 m_pos;
	vec2 m_size;
	vec2 m_sizeDelta;
};

//=============================================================================
// ComponentLife class
//=============================================================================
class ComponentLife: public Component {
public:
	ComponentLife(Entity* owner, int life, int timeToLive, int invencibleTime);

	virtual void run           ();
	virtual void receiveMessage(Message* message);
private:
	int m_life;
	int m_timeToLive;
	int m_invencibleTime;

	//Timers
	int m_lifeTimer;
	int m_hitTimer;
};

//=============================================================================
// ComponentInertialMove class
//=============================================================================
class ComponentInertialMove : public Component {
public:
	ComponentInertialMove(Entity* owner, const vec2& direction, float speed, bool hasInertia) : Component(owner), m_direction(direction), m_speed(speed), m_hasInertia(hasInertia) {}
	
	virtual void run();
	virtual void receiveMessage(Message* message);
private:
	vec2  m_direction;
	float m_speed;
	bool  m_hasInertia;
};

//=============================================================================
// ComponentRenderable class
//=============================================================================
class ComponentRenderable : public Component {
public:
	ComponentRenderable(Entity* owner, const char* texture, int priority = 2, float alpha= 1.0f, const char* hitTexture = nullptr, int hitTime = 0);
	~ComponentRenderable();

	virtual void init          ();
	virtual void run           ();
	virtual void receiveMessage(Message* message);
private:
	Sprite*     m_sprite;
	const char* m_texture;
	int         m_priority;
	float       m_alpha;
	const char* m_hitTexture;
	int         m_hitTime;

	//Timers
	int     m_hitTimer;
};

//=============================================================================
// ComponentPlayerController class
//=============================================================================
class ComponentPlayerController : public Component, public IInputManager::IListener {
public:
	ComponentPlayerController(Entity* owner, float speed) : Component(owner), m_speed(speed) {}
	~ComponentPlayerController();

	virtual void init   ();
	virtual bool onEvent(const IInputManager::Event&);
private:
	float m_speed;
};

//=============================================================================
// ComponentWeapon class
//=============================================================================
class ComponentWeapon : public Component {
public:
	ComponentWeapon(Entity* owner, TWeapon type, int fireRate, int reloadTime, int bullets, int bulletSpeed, int bulletDamage, int bulletRange, bool isAutomatic, const char* soundFilename = nullptr);

	virtual void init          ();
	virtual void run           ();
	virtual void receiveMessage(Message* message);
private:
	TWeapon     m_type;
	int         m_fireRate;
	int         m_reloadTime;
	int         m_bullets;
	int         m_bulletSpeed;
	int         m_bulletDamage;
	int         m_bulletRange;
	bool        m_isAutomatic;
	const char* m_soundFilename;
	vec2        m_aimDirection;
	int         m_currentBullets;
	bool        m_isFiring;
	uint        m_soundId;

	//Timers
	int m_fireTimer;
	int m_reloadTimer;
};

//=============================================================================
// ComponentExplossive class
//=============================================================================
class ComponentExplossive : public Component, public IInputManager::IListener {
public:
	ComponentExplossive(Entity* owner, bool isActivatedRemotely, Component* weapon = nullptr) : Component(owner), m_isActivatedRemotely(isActivatedRemotely), m_weapon(weapon) {}
	~ComponentExplossive();

	virtual void init          ();
	virtual void receiveMessage(Message* message);
	virtual bool onEvent       (const IInputManager::Event&);
private:
	bool       m_isActivatedRemotely;
	Component* m_weapon;
};

//=============================================================================
// C_Target class
//=============================================================================
//Me lo salto por si lo muevo al HUD
//Convertir a HUD
class C_Target : public Component, public IInputManager::IListener {
public:
	C_Target(Entity* owner, char* texture);
	~C_Target();
	//virtual void receiveMessage(Message* message);

	virtual void activate();
	virtual void deactivate();

	virtual bool onEvent(const IInputManager::Event&);
private:
	Sprite* m_sprite;
};

//=============================================================================
// ComponentAIMelee class
//=============================================================================
class ComponentAIMelee : public Component {
public:
	ComponentAIMelee(Entity* owner, Entity* player, float speed, float maxDistance);
	
	virtual void run();
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
	
	virtual void run();
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
	ComponentAIFire(Entity* owner, const std::vector<vec2> fireDirections) : Component(owner), m_fireDirections(fireDirections) {}
	
	virtual void init          ();
	virtual void run           ();
	virtual void receiveMessage(Message* message);
private:
	Entity*           m_player;
	std::vector<vec2> m_fireDirections;
	int               m_currentFireDirection;
};

//=============================================================================
// ComponentCollider class
//=============================================================================
class ComponentCollider : public Component {
public:
	ComponentCollider(Entity* owner, TColliderType type, TFaction faction, int deltaLife) : Component(owner), m_type(type), m_faction(faction), m_deltaLife(deltaLife) {}
	
	virtual void run           ();
	virtual void receiveMessage(Message* message);
private:
	TColliderType m_type;
	TFaction      m_faction;
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
class ComponentHUD : public Component {
public:
	ComponentHUD(Entity* owner) : Component(owner) {}
	~ComponentHUD();

	virtual void init();
	virtual void run ();
private:
	Text*   m_life;
	Text*   m_score;
	Text*   m_ammo;
	Text*   m_fps;

	//ver si cambiar a Renderable
	Sprite* m_reloadAnim;
};
