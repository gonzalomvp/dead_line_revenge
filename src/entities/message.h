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

struct MessageTransformChanged : public Message {
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

struct MessageGetCollider : public Message {
	Component::TColliderType type;
	Component::TFaction faction;
	vec2 center;
	vec2 size;
	int deltaLife;
};

struct MessageCollision: public Message {
	Entity* other = nullptr;
	Component::TFaction faction;
	int deltaLife;
};

struct MessageWeaponChange : public Message {
	Component::TWeapon weapon;
};

struct MessageDestroy : public Message {
};
