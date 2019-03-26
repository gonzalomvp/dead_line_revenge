#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CGoToPlayerPositionAction : public CBehaviorNode {
public:
	CGoToPlayerPositionAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner), m_fArriveDistance(0.0f), m_bKeepFollowing(false) {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	vec2 mTargetPos;
	float m_fArriveDistance;
	bool m_bKeepFollowing;
};

