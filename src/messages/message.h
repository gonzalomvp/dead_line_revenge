#pragma once

#include "components/ColliderComponent.h"
#include "components/WeaponComponent.h"

struct TMessage {
	virtual ~TMessage() {}
};

struct TMessageGetLife : public TMessage {
	int currentLife = 0;
};

struct TMessageChangeLife : public TMessage {
	int deltaLife = 0;
};

struct TMessageGetMovementDir : public TMessage {
	vec2 dir = vmake(0.0f, 0.0f);
};

struct TMessageSetMovementDir : public TMessage {
	vec2 dir = vmake(0.0f, 0.0f);
};

struct TMessageSetMovementSpeed : public TMessage {
	float speed = 0.0f;
};

struct TMessageReload : public TMessage {
};

struct TMessageFire : public TMessage {
	bool isFiring   = false;
};

struct TMessageSetAimDirection : public TMessage {
	vec2 direction = vmake(0.0f, 0.0f);
};

struct TMessageGetAimDirection : public TMessage {
	vec2 direction = vmake(0.0f, 0.0f);
};

struct TMessageAmmoInfo : public TMessage {
	int   currentAmmo     = 0;
	int   totalAmmo       = 0;
	float reloadPercent   = 1.0f;
};

struct TMessageCheckCollision : public TMessage {
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

struct TMessageWeaponChange : public TMessage {
	CWeaponComponent::EType eWeaponType;
};

struct TMessageDestroy : public TMessage {
};

struct TMessageChangeSprite: public TMessage {
	std::string texture = "";
};

struct TMessageEnableAI: public TMessage {
	bool enable = true;
};

struct TMessageShowHUDTMessage : public TMessage {
	std::string message = "";
	float       time    = 0.0f;
};