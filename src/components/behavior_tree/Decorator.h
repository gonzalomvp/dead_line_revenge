#pragma once

#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CDecorator : public CBehavior {
public:
	CDecorator(CBehaviorTreeComponent* _pOwnerComponent) : CBehavior(_pOwnerComponent) {}
	~CDecorator();

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;
	virtual void abort()                            override;

protected:
	CBehavior* m_pChildBehavior;
};