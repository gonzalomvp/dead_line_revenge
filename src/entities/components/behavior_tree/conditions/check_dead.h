#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class CheckDead : public Behavior {
public:
	CheckDead(BehaviorTree* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
};