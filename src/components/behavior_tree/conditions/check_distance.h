#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CCheckDistanceCondition : public CBehaviorNode {
public:
	CCheckDistanceCondition(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner) {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual Status update(float step) override;

private:
	int mMinDistance;
	bool m_bNegate;
};