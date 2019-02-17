#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class GoToPlayerPosition : public Behavior {
public:
	GoToPlayerPosition(BehaviorTree* owner, float speed) : Behavior(owner), mSpeed(speed) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	vec2 mTargetPos;
	float mSpeed;
};

