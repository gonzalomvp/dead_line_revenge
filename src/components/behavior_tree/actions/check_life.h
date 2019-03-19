#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CCheckLifeCondition : public CBehaviorNode {
public:
	CCheckLifeCondition(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner), m_iLife(0) {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual Status update(float step) override;

private:
	int m_iLife;
};

