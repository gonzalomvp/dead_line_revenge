#pragma once

#include "components/Component.h"

class CColliderComponent : public CComponent {
public:
	enum EType {
		ERectCollider,
		ECircleCollider,
	};

	enum EChannel {
		EPlayerCollider = 1 << 0,
		EEnemyCollider = 1 << 1,
		EPlayerWeaponCollider = 1 << 2,
		EEnemyWeaponCollider = 1 << 3,
		EBoundariesCollider = 1 << 4,
		EPickupCollider = 1 << 5,
		ENoneCollider = 0,
	};

	CColliderComponent(CEntity* _pOwner, EType _eType, int _iDamage, int _iChannelMask, int _iChannelMaskResponse)
	: CComponent(_pOwner)
	, m_eType(_eType)
	, m_iDamage(_iDamage)
	, m_iChannelMask(_iChannelMask)
	, m_iChannelMaskResponse(_iChannelMaskResponse)
	{}

	// CComponent
	virtual void receiveMessage(TMessage* _pMessage) override;

private:
	EType m_eType;
	int   m_iDamage;
	int   m_iChannelMask;
	int   m_iChannelMaskResponse;
};
