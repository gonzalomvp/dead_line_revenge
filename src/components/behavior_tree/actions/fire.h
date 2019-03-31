#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CFire : public CBehavior {
public:
	CFire(CBehaviorTreeComponent* _pOwnerComponent)
	: CBehavior(_pOwnerComponent)
	{}

protected:
	// CBehavior
	virtual EStatus onUpdate(float _fDeltaTime) override;
};

