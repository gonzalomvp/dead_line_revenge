#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class AimPlayer : public Behavior {
public:
	AimPlayer(BehaviorTree* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
};

