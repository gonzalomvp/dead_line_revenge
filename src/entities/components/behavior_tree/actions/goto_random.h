#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;

class CGoToRandomPosition : public Behavior {
public:
	CGoToRandomPosition(CBehaviorTreeComponent* owner) : Behavior(owner) {}
	
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	vec2 mTargetPos;
	float mSpeed;
};

