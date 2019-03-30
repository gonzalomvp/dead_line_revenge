#pragma once

#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CDecorator : public CBehavior {
public:
	CDecorator(CBehaviorTreeComponent* owner) : CBehavior(owner) {}
	~CDecorator();

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;
	virtual void abort() override;

protected:
	CBehavior* m_pChildNode;
};