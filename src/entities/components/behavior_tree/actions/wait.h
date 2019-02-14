#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class Wait : public Behavior {
public:
	Wait(BehaviorTree* owner, int waitTime) : Behavior(owner), mWaitTime(waitTime) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	int mWaitTime;
	int mWaitTimer;
};

