#pragma once
#include "components/behavior_tree/Condition.h"

class CBehaviorTreeComponent;

class CCheckIsAtDistance : public CCondition{
public:
	CCheckIsAtDistance(CBehaviorTreeComponent* owner) : CCondition(owner), m_fDistance(0.0f), m_sBlackboardKey("") {}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	
	// CConditionNode
	virtual bool check(float step) override;

private:
	float m_fDistance;
	std::string m_sBlackboardKey;
};