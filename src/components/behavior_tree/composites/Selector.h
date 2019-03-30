#pragma once

#include "components/behavior_tree/Composite.h"

class CBehaviorTreeComponent;

class CSelector : public CComposite {
public:
	CSelector(CBehaviorTreeComponent* owner) : CComposite(owner) {}

protected:
	virtual void onEnter() override;
	virtual EStatus onUpdate(float step) override;

	int mCurrentChild;
};
