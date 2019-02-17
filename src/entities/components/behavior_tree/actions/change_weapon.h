#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class ChangeWeapon : public Behavior {
public:
	ChangeWeapon(BehaviorTree* owner, int weaponId) : Behavior(owner), mWeaponID(weaponId) {}

protected:
	virtual Status update(float step) override;

private:
	int mWeaponID;
};

