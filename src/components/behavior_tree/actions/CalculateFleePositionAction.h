#pragma once

#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CCalculateFleePositionAction : public CBehaviorNode {
public:
	CCalculateFleePositionAction(CBehaviorNodeTreeComponent* owner)
	: CBehaviorNode(owner)
	, m_sFromBlackboardKey("")
	, m_sResultBlackboardKey("")
	, m_fDistance(0.0f)
	{}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual Status update(float step) override;

private:
	std::string m_sFromBlackboardKey;
	std::string m_sResultBlackboardKey;
	float m_fDistance;
};

