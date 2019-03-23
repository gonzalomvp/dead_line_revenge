#pragma once

#include "components/component.h"
#include "components/WeaponComponent.h"

class CWeaponPickupComponent : public Component {
public:
	CWeaponPickupComponent(Entity* _pOwner, CWeaponComponent::EType _eWeaponType)
	: Component(_pOwner)
	, m_eWeaponType(_eWeaponType)
	{}

	// CComponent
	virtual void receiveMessage(Message* _pMessage) override;

private:
	CWeaponComponent::EType m_eWeaponType;
};
