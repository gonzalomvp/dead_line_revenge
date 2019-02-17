#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class GoToRandomPosition : public Behavior {
public:
	GoToRandomPosition(BehaviorTree* owner, float speed) : Behavior(owner), mSpeed(speed) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	vec2 mTargetPos;
	float mSpeed;
};

