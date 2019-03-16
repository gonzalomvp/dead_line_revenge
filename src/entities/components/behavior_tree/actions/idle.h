#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class Idle : public CBehaviorNode {
public:
	Idle(CBehaviorNodeTreeComponent* owner, bool enabled) : CBehaviorNode(owner), mEnabled(enabled) {}

protected:
	virtual Status update(float step) override;

private:
	bool mEnabled;
};
