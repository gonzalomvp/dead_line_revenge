#pragma once
#include "group.h"

class BehaviorTree;

class Repeat : public Group {
public:
	Repeat(BehaviorTree* owner, int numRepeats) : Group(owner), mNumRepeats(numRepeats) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mNumRepeats;
	int mCount;
};
