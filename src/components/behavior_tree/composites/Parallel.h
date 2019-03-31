#pragma once

#include "components/behavior_tree/Composite.h"

class CBehaviorTreeComponent;

class CParallel : public CComposite {
public:
	CParallel(CBehaviorTreeComponent* _pOwnerComponent) : CComposite(_pOwnerComponent) {}

protected:
	// CBehavior
	virtual EStatus onUpdate(float _fDeltaTime) override;
};
