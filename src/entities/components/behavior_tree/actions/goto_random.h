#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class GoToRandomPosition : public Behavior {
public:
	GoToRandomPosition(CBehaviorTreeComponent* owner, float speed) : Behavior(owner), mSpeed(speed) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	vec2 mTargetPos;
	float mSpeed;
};

