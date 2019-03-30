#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CCalculateRandomPositionAction : public CBehaviorNode {
public:
	CCalculateRandomPositionAction(CBehaviorNodeTreeComponent* owner)
	: CBehaviorNode(owner)
	, m_sResultBlackboardKey("")
	{}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual Status update(float step) override;

private:
	std::string m_sResultBlackboardKey;
};

