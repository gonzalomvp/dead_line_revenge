#pragma once
#include "components/behavior_tree/Condition.h"

class CBehaviorTreeComponent;

class CCheckLife : public CCondition {
public:
	CCheckLife(CBehaviorTreeComponent* _pOwnerComponent) : CCondition(_pOwnerComponent), m_iLife(0) {}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	// CConditionNode
	virtual bool check(float _fDeltaTime) override;

private:
	int m_iLife;
};

