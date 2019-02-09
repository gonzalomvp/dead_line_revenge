#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class CheckHit : public Behavior {
public:
	CheckHit(BehaviorTree* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
};