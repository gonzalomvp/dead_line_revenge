#pragma once

#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CCondition : public CBehavior {
public:
	CCondition(CBehaviorTreeComponent* _pOwnerComponent) : CBehavior(_pOwnerComponent) {}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	virtual bool check(float _fDeltaTime) = 0;

	// CBehavior
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	bool m_bNegate;
};