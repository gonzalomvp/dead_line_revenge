#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CRotateAimAction : public CBehaviorNode {
public:
	CRotateAimAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner), m_fAngle(0.0f) {}

protected:
	virtual Status update(float step) override;

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

private:
	float m_fAngle;
};

