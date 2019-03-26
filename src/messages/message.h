#pragma once
#include "./components/ColliderComponent.h"
#include "./components/WeaponComponent.h"

struct Message {
	virtual ~Message() {}
};

struct MessageGetLife : public Message {
	int currentLife = 0;
};

struct MessageChangeLife : public Message {
	int deltaLife = 0;
};

struct MessageDamaged : public Message {
	int deltaLife = 0;
};

struct MessageSetMovementDir : public Message {
	vec2 dir = vmake(0.0f, 0.0f);
};

struct MessageSetMovementSpeed : public Message {
	float speed = 0.0f;
};

struct MessageReload : public Message {
};

struct MessageFire : public Message {
	bool isFiring   = false;
};

struct MessageSetAimDirection : public Message {
	vec2 direction = vmake(0.0f, 0.0f);
};

struct MessageGetAimDirection : public Message {
	vec2 direction = vmake(0.0f, 0.0f);
};

struct MessageAmmoInfo : public Message {
	int   currentAmmo     = 0;
	int   totalAmmo       = 0;
	float reloadPercent   = 1.0f;
};

struct MessageCheckCollision : public Message {
	CColliderComponent::EType type;
	vec2                      center                    = vmake(0.0f, 0.0f);
	vec2                      size                      = vmake(0.0f, 0.0f);
	int                       deltaLife                 = 0;
	int                       collisionChannel          = 0;
	int                       collisionChannelsResponse = 0;
	Entity*                   other                     = nullptr;
	bool                      overlap                   = false;
	bool                      bounceX                   = false;
	bool                      bounceY                   = false;
};

struct MessageWeaponChange : public Message {
	CWeaponComponent::EType eWeaponType;
};

struct MessageDestroy : public Message {
};

struct MessageChangeSprite: public Message {
	std::string texture = "";
};

struct MessageEnableAI: public Message {
	bool enable = true;
};

struct MessageShowHUDMessage : public Message {
	std::string message = "";
	int         time = 0;
};