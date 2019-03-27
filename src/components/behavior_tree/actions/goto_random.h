#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CGoToRandomPositionAction : public CBehaviorNode {
public:
	CGoToRandomPositionAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner), m_fArriveDistance(0.0f) {}
	
	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;
	virtual void abort()                          override;

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	vec2 mTargetPos;
	float m_fArriveDistance;
};

