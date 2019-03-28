#pragma once
#include "components/behavior_tree/ConditionNode.h"

class CBehaviorNodeTreeComponent;

class CCheckDistanceCondition : public CConditionNode{
public:
	CCheckDistanceCondition(CBehaviorNodeTreeComponent* owner) : CConditionNode(owner) {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	
	// CConditionNode
	virtual bool check(float step) override;

private:
	int mMinDistance;
};