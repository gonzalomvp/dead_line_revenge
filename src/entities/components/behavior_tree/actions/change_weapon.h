#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class ChangeWeapon : public Behavior {
public:
	ChangeWeapon(CBehaviorTreeComponent* owner, const std::string& weaponName) : Behavior(owner), mWeaponName(weaponName) {}

protected:
	virtual Status update(float step) override;

private:
	std::string mWeaponName;
};

