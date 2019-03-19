#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CFireAction : public CBehaviorNode {
public:
	CFireAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

protected:
	virtual Status update(float step) override;

private:
};

