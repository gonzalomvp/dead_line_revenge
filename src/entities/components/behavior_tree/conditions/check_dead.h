#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class CheckDead : public Behavior {
public:
	CheckDead(CBehaviorTreeComponent* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
};