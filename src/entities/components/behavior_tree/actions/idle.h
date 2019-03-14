#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class Idle : public Behavior {
public:
	Idle(CBehaviorTreeComponent* owner, bool enabled) : Behavior(owner), mEnabled(enabled) {}

protected:
	virtual Status update(float step) override;

private:
	bool mEnabled;
};
