#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CGoToRandomPositionAction : public CBehaviorNode {
public:
	CGoToRandomPositionAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}
	
	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	vec2 mTargetPos;
	float mSpeed;
};

