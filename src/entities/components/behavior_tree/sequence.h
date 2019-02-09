#pragma once
#include "group.h"

class BehaviorTree;

class Sequence : public Group {
public:
	Sequence(BehaviorTree* owner) : Group(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mCurrentChild;
};
