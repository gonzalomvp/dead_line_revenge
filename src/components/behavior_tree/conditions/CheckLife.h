#pragma once
#include "components/behavior_tree/Condition.h"

class CBehaviorTreeComponent;

class CCheckLife : public CCondition {
public:
	CCheckLife(CBehaviorTreeComponent* owner) : CCondition(owner), m_iLife(0) {}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	// CConditionNode
	virtual bool check(float step) override;

private:
	int m_iLife;
};

