#pragma once
#include "components/behavior_tree/ConditionNode.h"

class CBehaviorNodeTreeComponent;

class CCheckIsAtDistanceCondition : public CConditionNode{
public:
	CCheckIsAtDistanceCondition(CBehaviorNodeTreeComponent* owner) : CConditionNode(owner), m_fDistance(0.0f), m_sBlackboardKey("") {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	
	// CConditionNode
	virtual bool check(float step) override;

private:
	float m_fDistance;
	std::string m_sBlackboardKey;
};