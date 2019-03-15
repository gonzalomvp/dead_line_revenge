#pragma once
#include "group.h"

class CBehaviorTreeComponent;

class CSelector : public Group {
public:
	CSelector(CBehaviorTreeComponent* owner) : Group(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mCurrentChild;
};
