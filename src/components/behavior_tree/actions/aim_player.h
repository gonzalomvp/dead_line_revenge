#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CAimToPlayerAction : public CBehaviorNode {
public:
	CAimToPlayerAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

protected:
	virtual Status update(float step) override;
};

