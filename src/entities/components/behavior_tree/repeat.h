#pragma once
#include "group.h"

class CBehaviorTreeComponent;

class Repeat : public Group {
public:
	Repeat(CBehaviorTreeComponent* owner, int numRepeats) : Group(owner), mNumRepeats(numRepeats) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mNumRepeats;
	int mCount;
};
