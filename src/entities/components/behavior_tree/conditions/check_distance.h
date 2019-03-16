#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CheckDistance : public CBehaviorNode {
public:
	CheckDistance(CBehaviorNodeTreeComponent* owner, float minDistance) : CBehaviorNode(owner), mMinDistance(minDistance) {}

protected:
	virtual Status update(float step) override;

private:
	float mMinDistance;
};