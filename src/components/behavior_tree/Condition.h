#pragma once

#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CCondition : public CBehavior {
public:
	CCondition(CBehaviorTreeComponent* owner) : CBehavior(owner) {}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual EStatus onUpdate(float step) override;
	virtual bool check(float step) = 0;

private:
	bool m_bNegate;
};