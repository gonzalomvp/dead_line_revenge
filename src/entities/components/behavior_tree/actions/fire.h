#pragma once
#include "entities/components/behavior_tree/behavior.h"

class BehaviorTree;

class Fire : public Behavior {
public:
	Fire(BehaviorTree* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;

private:
};

