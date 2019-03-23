#pragma once

#include "components/component.h"

class CWeaponPickupComponent : public Component {
public:
	CWeaponPickupComponent(Entity* _pOwner, ComponentWeapon::EType _eWeaponType)
	: Component(_pOwner)
	, m_eWeaponType(_eWeaponType)
	{}

	// CComponent
	virtual void receiveMessage(Message* _pMessage) override;

private:
	ComponentWeapon::EType m_eWeaponType;
};
