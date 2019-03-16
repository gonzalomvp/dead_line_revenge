#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class Death : public CBehaviorNode {
public:
	Death(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;
	virtual void onExit() override;
	
private:
	float mRespawnTimer;
};
