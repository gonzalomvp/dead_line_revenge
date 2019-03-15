#pragma once
#include "group.h"

class CBehaviorTreeComponent;

class CSequence : public Group {
public:
	CSequence(CBehaviorTreeComponent* owner) : Group(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mCurrentChild;
};
