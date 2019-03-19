#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;
class AlignToMovementSteering;
class PursueSteering;

class Chase : public CBehaviorNode {
public:
	Chase(CBehaviorNodeTreeComponent* owner, float arriveDistance, float lostDistance);
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
