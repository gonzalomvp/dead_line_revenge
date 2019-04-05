#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CWait : public CBehavior {
public:
	CWait(CBehaviorTreeComponent* _pOwnerComponent)
	: CBehavior(_pOwnerComponent)
	, m_fWaitTime(0.0f)
	, m_fWaitTimer(0.0f)
	, m_fRandomDeviation(0.0f)
	{}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	// CBehavior
	virtual void    onEnter()                   override;
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	float m_fWaitTime;
	float m_fWaitTimer;
	float m_fRandomDeviation;
};

