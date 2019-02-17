#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class CheckLife : public Behavior {
public:
	CheckLife(BehaviorTree* owner, int life) : Behavior(owner), mLife(life) {}

protected:
	virtual Status update(float step) override;

private:
	int mLife;
};

