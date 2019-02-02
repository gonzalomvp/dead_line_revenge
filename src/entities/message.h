#pragma once
#include "./components/component.h"

struct Message {
	virtual ~Message() {}
};

struct MessageGetTransform : public Message {
	vec2 pos  = vmake(0.0f, 0.0f);
	vec2 size = vmake(0.0f, 0.0f);
};

struct MessageSetTransform : public Message {
	vec2 pos  = vmake(0.0f, 0.0f);
	vec2 size = vmake(0.0f, 0.0f);
};

struct MessageGetLife : public Message {
	int currentLife = 0;
};

struct MessageChangeLife : public Message {
	int deltaLife = 0;
};

struct MessageAddMovement : public Message {
	vec2 dir = vmake(0.0f, 0.0f);
};

struct MessageReload : public Message {
};

struct MessageFire : public Message {
	bool isFiring   = false;
	bool isFireDone = false;
};

struct MessageAimDirection : public Message {
	vec2 direction = vmake(0.0f, 0.0f);
};

struct MessageAmmoInfo : public Message {
	int   currentAmmo     = 0;
	int   totalAmmo       = 0;
	float reloadPercent   = 1.0f;
};

struct MessageCheckCollision : public Message {
	ComponentCollider::TColliderType type;
	vec2                             center                    = vmake(0.0f, 0.0f);
	vec2                             size                      = vmake(0.0f, 0.0f);
	int                              deltaLife                 = 0;
	int                              collisionChannel          = 0;
	int                              collisionChannelsResponse = 0;
	ptr<Entity>                      other                     = nullptr;
	bool                             overlap                   = false;
	vec2                             bounceDirection           = vmake(0.0f, 0.0f);
};

struct MessageWeaponChange : public Message {
	ComponentWeapon::TWeaponData weaponData;
};

struct MessageDestroy : public Message {
};
