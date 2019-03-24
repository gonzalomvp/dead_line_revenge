#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CChangeAimDirectionAction : public CBehaviorNode {
public:
	CChangeAimDirectionAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

protected:
	virtual Status update(float step) override;
};

