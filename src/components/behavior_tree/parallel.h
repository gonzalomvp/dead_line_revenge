#pragma once

#include "components/behavior_tree/group.h"

class CBehaviorNodeTreeComponent;

class CParallelNode : public CGroupNode {
public:
	CParallelNode(CBehaviorNodeTreeComponent* owner) : CGroupNode(owner) {}

protected:
	virtual Status update(float step) override;
};
