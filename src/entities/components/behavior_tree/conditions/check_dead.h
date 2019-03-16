#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CheckDead : public CBehaviorNode {
public:
	CheckDead(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

protected:
	virtual Status update(float step) override;
};