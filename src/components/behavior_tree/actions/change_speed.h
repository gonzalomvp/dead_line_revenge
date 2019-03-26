#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CChangeSpeedAction : public CBehaviorNode {
public:
	CChangeSpeedAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner), m_fSpeed(0.0f) {}

protected:
	virtual Status update(float step) override;

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

private:
	float m_fSpeed;
};

