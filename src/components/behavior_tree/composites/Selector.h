#pragma once

#include "components/behavior_tree/Composite.h"

class CBehaviorTreeComponent;

class CSelector : public CComposite {
public:
	CSelector(CBehaviorTreeComponent* _pOwnerComponent) : CComposite(_pOwnerComponent) {}

protected:
	// CBehavior
	virtual void    onEnter()                   override;
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	uint16_t m_uCurrentChild;
};
