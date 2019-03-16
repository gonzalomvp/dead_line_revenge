#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CheckHit : public CBehaviorNode {
public:
	CheckHit(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

protected:
	virtual Status update(float step) override;
};