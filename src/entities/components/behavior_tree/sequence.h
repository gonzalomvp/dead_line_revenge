#pragma once
#include "group.h"

class CBehaviorTreeComponent;

class Sequence : public Group {
public:
	Sequence(CBehaviorTreeComponent* owner) : Group(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int mCurrentChild;
};
