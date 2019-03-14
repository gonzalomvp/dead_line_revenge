#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class Fire : public Behavior {
public:
	Fire(CBehaviorTreeComponent* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;

private:
};

