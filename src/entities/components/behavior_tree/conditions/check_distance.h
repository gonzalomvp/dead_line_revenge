#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class CheckDistance : public Behavior {
public:
	CheckDistance(CBehaviorTreeComponent* owner, float minDistance) : Behavior(owner), mMinDistance(minDistance) {}

protected:
	virtual Status update(float step) override;

private:
	float mMinDistance;
};