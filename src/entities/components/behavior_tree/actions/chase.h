#pragma once
#include "entities/components/behavior_tree/behavior.h"

class CBehaviorTreeComponent;
class AlignToMovementSteering;
class PursueSteering;

class Chase : public Behavior {
public:
	Chase(CBehaviorTreeComponent* owner, float arriveDistance, float lostDistance);
	~Chase();

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;
	virtual void onExit() override;

private:
	float                    mArriveDistance;
	float                    mLostDistance;
	AlignToMovementSteering* mAlignToMovementSteering;
	PursueSteering*          mPursueSteering;
};
