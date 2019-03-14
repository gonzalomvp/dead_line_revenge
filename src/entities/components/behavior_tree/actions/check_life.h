#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class CheckLife : public Behavior {
public:
	CheckLife(CBehaviorTreeComponent* owner, int life) : Behavior(owner), mLife(life) {}

protected:
	virtual Status update(float step) override;

private:
	int mLife;
};

