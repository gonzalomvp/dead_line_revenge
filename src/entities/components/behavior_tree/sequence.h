#pragma once
#include "group.h"

class CBehaviorNodeTreeComponent;

class CSequenceNode : public CGroupNode {
public:
	CSequenceNode(CBehaviorNodeTreeComponent* owner) : CGroupNode(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mCurrentChild;
};
