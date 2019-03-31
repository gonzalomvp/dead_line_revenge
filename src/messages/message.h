#pragma once

#include "components/ColliderComponent.h"
#include "components/WeaponComponent.h"

struct TMessage {
	virtual ~TMessage() {}
};

struct TMessageGetLife : public TMessage {
	int iCurrentLife = 0;
};

struct TMessageChangeLife : public TMessage {
	int iDeltaLife = 0;
};

struct TMessageGetMovementDir : public TMessage {
	vec2 v2Dir = vmake(0.0f, 0.0f);
};

struct TMessageSetMovementDir : public TMessage {
	vec2 v2Dir = vmake(0.0f, 0.0f);
};

struct TMessageSetMovementSpeed : public TMessage {
	float fSpeed = 0.0f;
};

struct TMessageReload : public TMessage {
};

struct TMessageFire : public TMessage {
	bool bIsFiring = false;
};

struct TMessageSetAimDirection : public TMessage {
	vec2 v2Dir = vmake(0.0f, 0.0f);
};

struct TMessageGetAimDirection : public TMessage {
	vec2 v2Dir = vmake(0.0f, 0.0f);
};

struct TMessageAmmoInfo : public TMessage {
	int   iCurrentAmmo     = 0;
	int   iTotalAmmo       = 0;
	float fReloadPercent   = 1.0f;
};

struct TMessageCheckCollision : public TMessage {
	CColliderComponent::EType eType;
	vec2                      v2Center             = vmake(0.0f, 0.0f);
	vec2                      v2Size               = vmake(0.0f, 0.0f);
	int                       iDamage              = 0;
	int                       iChannelMask         = 0;
	int                       iChannelMaskResponse = 0;
	CEntity*                  pOther               = nullptr;
	bool                      bIsOverlapping       = false;
};

struct TMessageWeaponChange : public TMessage {
	CWeaponComponent::EType eWeaponType;
};

struct TMessageDestroy : public TMessage {
};

struct TMessageChangeSprite: public TMessage {
	std::string sTexture = "";
};

struct TMessageEnableAI: public TMessage {
	bool bWantsEnable = true;
};

struct TMessageShowHUDTMessage : public TMessage {
	std::string sMessage = "";
	float       fTime    = 0.0f;
};