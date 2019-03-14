#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class CheckHit : public Behavior {
public:
	CheckHit(CBehaviorTreeComponent* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
};