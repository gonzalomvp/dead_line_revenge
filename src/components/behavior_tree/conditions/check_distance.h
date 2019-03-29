#pragma once
#include "components/behavior_tree/ConditionNode.h"

class CBehaviorNodeTreeComponent;

class CIsAtPositionCondition : public CConditionNode{
public:
	CIsAtPositionCondition(CBehaviorNodeTreeComponent* owner) : CConditionNode(owner), m_fRadius(0.0f), m_sBlackboardKey("") {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	
	// CConditionNode
	virtual bool check(float step) override;

private:
	float m_fRadius;
	std::string m_sBlackboardKey;
};