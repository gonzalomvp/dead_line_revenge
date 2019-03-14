#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class GoToPlayerPosition : public Behavior {
public:
	GoToPlayerPosition(CBehaviorTreeComponent* owner, float speed) : Behavior(owner), mSpeed(speed) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	vec2 mTargetPos;
	float mSpeed;
};

