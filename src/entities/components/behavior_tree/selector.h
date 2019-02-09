#pragma once
#include "group.h"

class BehaviorTree;

class Selector : public Group {
public:
	Selector(BehaviorTree* owner) : Group(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mCurrentChild;
};
