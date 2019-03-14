#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class Death : public Behavior {
public:
	Death(CBehaviorTreeComponent* owner) : Behavior(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;
	virtual void onExit() override;
	
private:
	float mRespawnTimer;
};
