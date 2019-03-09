#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class RotateAim : public Behavior {
public:
	RotateAim(BehaviorTree* owner, float angle) : Behavior(owner), mAngle(angle) {}

protected:
	virtual Status update(float step) override;

private:
	float mAngle;
};

