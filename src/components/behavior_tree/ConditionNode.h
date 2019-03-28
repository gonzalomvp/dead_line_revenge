#pragma once

#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CConditionNode : public CBehaviorNode {
public:
	CConditionNode(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual Status update(float step) override;
	virtual bool check(float step) = 0;

private:
	bool m_bNegate;
};