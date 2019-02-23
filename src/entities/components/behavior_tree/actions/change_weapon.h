#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class ChangeWeapon : public Behavior {
public:
	ChangeWeapon(BehaviorTree* owner, const std::string& weaponName) : Behavior(owner), mWeaponName(weaponName) {}

protected:
	virtual Status update(float step) override;

private:
	std::string mWeaponName;
};

