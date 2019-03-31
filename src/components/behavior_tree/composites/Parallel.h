#pragma once

#include "components/behavior_tree/Composite.h"

class CBehaviorTreeComponent;

class CParallel : public CComposite {
public:
	CParallel(CBehaviorTreeComponent* owner) : CComposite(owner) {}

protected:
	virtual EStatus onUpdate(float _fDeltaTime) override;
};
