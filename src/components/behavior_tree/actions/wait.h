#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CWaitAction : public CBehaviorNode {
public:
	CWaitAction(CBehaviorNodeTreeComponent* owner) : CBehaviorNode(owner), m_fWaitTime(0.0f), m_fWaitTimer(0.0f), m_fRandomDeviation(0.0f) {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	float m_fWaitTime;
	float m_fWaitTimer;
	float m_fRandomDeviation;
};

