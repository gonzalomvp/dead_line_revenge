#pragma once

#include "components/Component.h"
#include "components/WeaponComponent.h"

class CWeaponPickupComponent : public CComponent {
public:
	CWeaponPickupComponent(Entity* _pOwner, CWeaponComponent::EType _eWeaponType)
	: CComponent(_pOwner)
	, m_eWeaponType(_eWeaponType)
	{}

	// CComponent
	virtual void receiveMessage(TMessage* _pMessage) override;

private:
	CWeaponComponent::EType m_eWeaponType;
};
