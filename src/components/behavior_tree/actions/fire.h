#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CFire : public CBehavior {
public:
	CFire(CBehaviorTreeComponent* owner) : CBehavior(owner) {}

protected:
	virtual EStatus onUpdate(float step) override;

private:
};

