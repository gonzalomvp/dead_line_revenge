#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CGoToPlayerPositionAction : public CBehaviorNode {
public:
	CGoToPlayerPositionAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner), m_fSpeed(0.0f) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	vec2 mTargetPos;
	float m_fSpeed;
};

