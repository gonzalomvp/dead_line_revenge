#pragma once
#include "group.h"

class CBehaviorTreeComponent;

class Selector : public Group {
public:
	Selector(CBehaviorTreeComponent* owner) : Group(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mCurrentChild;
};
