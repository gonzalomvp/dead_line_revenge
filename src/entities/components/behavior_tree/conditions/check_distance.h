#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class CheckDistance : public Behavior {
public:
	CheckDistance(BehaviorTree* owner, float minDistance) : Behavior(owner), mMinDistance(minDistance) {}

protected:
	virtual Status update(float step) override;

private:
	float mMinDistance;
};