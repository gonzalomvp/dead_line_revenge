#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class Idle : public Behavior {
public:
	Idle(BehaviorTree* owner, bool enabled) : Behavior(owner), mEnabled(enabled) {}

protected:
	virtual Status update(float step) override;

private:
	bool mEnabled;
};
