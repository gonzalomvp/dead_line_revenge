#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class Hit : public CBehaviorNode {
public:
	Hit(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;
	virtual void onExit() override;

private:
	float mHitTimer;
};
