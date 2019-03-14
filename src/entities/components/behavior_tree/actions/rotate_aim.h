#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class RotateAim : public Behavior {
public:
	RotateAim(CBehaviorTreeComponent* owner, float angle) : Behavior(owner), mAngle(angle) {}

protected:
	virtual Status update(float step) override;

private:
	float mAngle;
};

