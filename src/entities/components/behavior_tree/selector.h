#pragma once
#include "group.h"

class CBehaviorNodeTreeComponent;

class CSelectorNode : public CGroupNode {
public:
	CSelectorNode(CBehaviorNodeTreeComponent* owner) : CGroupNode(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mCurrentChild;
};
