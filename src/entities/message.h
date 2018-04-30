#pragma once
#include "./components/component.h"

struct Message {
	virtual ~Message() {}
};

struct MessageGetTransform : public Message {
	vec2 pos;
	vec2 size;
};

struct MessageSetTransform : public Message {
	vec2 pos;
	vec2 size;
};

struct MessageGetLife : public Message {
	int currentLife;
};

struct MessageChangeLife : public Message {
	int deltaLife;
};

struct MessageAddMovement : public Message {
	vec2 dir;
};

struct MessageReload : public Message {
};

struct MessageFire : public Message {
	bool isFiring;
};

struct MessageFireDone : public Message {
};

struct MessageAimDirection : public Message {
	vec2 direction;
};

struct MessageAmmoInfo : public Message {
	int currentAmmo;
	int totalAmmo;
	float reloadPercent = 1.0f;
};





//struct MessageGetTargetPos : public Message {
//	vec2 pos;
//};

struct MessageCheckCollision : public Message {
	Component::TColliderType type;
	vec2 center;
	vec2 size;
	int deltaLife;
	int collisionChannel = 0;
	int collisionChannelsResponse = 0;
	Entity* other = nullptr;
	bool overlap = false;
	vec2 bounceDirection = vmake(0.0f, 0.0f);
};

//struct MessageGetCollider : public Message {
//	Component::TColliderType type;
//	Component::TFaction faction;
//	vec2 center;
//	vec2 size;
//	int deltaLife;
//	int collisionChannel;
//};

//struct MessageCollision: public Message {
//	Entity* other = nullptr;
//	Component::TFaction faction;
//	int deltaLife;
//	vec2 bounceDirection;
//};

struct MessageWeaponChange : public Message {
	Component::TWeaponData weaponData;
};

struct MessageDestroy : public Message {
};
