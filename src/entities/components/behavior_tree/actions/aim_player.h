#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class AimPlayer : public Behavior {
public:
	AimPlayer(CBehaviorTreeComponent* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
};

