#pragma once
#include "components/behavior_tree/Condition.h"

class CBehaviorTreeComponent;

class CCheckIsAtDistance : public CCondition{
public:
	CCheckIsAtDistance(CBehaviorTreeComponent* _pOwnerComponent) : CCondition(_pOwnerComponent), m_fDistance(0.0f), m_sBlackboardKey("") {}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	// CConditionNode
	virtual bool check(float _fDeltaTime) override;

private:
	float       m_fDistance;
	std::string m_sBlackboardKey;
};