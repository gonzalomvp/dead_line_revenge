#pragma once
#include "components/behavior_tree/Decorator.h"

class CBehaviorTreeComponent;

class CRepeat : public CDecorator {
public:
	CRepeat(CBehaviorTreeComponent* _pOwner) : CDecorator(_pOwner), m_iCounter(0) {}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual void onEnter() override;
	virtual EStatus onUpdate(float _fDeltaTime) override;

	int m_iTimes;
	int m_iCounter;
};
