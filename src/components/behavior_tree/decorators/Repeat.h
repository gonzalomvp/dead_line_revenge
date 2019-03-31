#pragma once
#include "components/behavior_tree/Decorator.h"

class CBehaviorTreeComponent;

class CRepeat : public CDecorator {
public:
	CRepeat(CBehaviorTreeComponent* _pOwnerComponent) : CDecorator(_pOwnerComponent), m_iCounter(0) {}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	// CBehavior
	virtual void    onEnter()                   override;
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	int m_iRepeatTimes;
	int m_iCounter;
};
