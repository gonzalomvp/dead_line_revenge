#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CWait : public CBehavior {
public:
	CWait(CBehaviorTreeComponent* owner) : CBehavior(owner), m_fWaitTime(0.0f), m_fWaitTimer(0.0f), m_fRandomDeviation(0.0f) {}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual void onEnter() override;
	virtual EStatus onUpdate(float step) override;

private:
	float m_fWaitTime;
	float m_fWaitTimer;
	float m_fRandomDeviation;
};

