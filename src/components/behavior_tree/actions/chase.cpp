#include "common/stdafx.h"
#include "chase.h"

#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"

Chase::Chase(CBehaviorNodeTreeComponent* owner, float arriveDistance, float lostDistance) : CBehaviorNode(owner), mArriveDistance(arriveDistance), mLostDistance(lostDistance) {

}

Chase::~Chase() {

	//delete mPursueSteering;
	//delete mAlignToMovementSteering;
}

void Chase::onEnter() {

}

Status Chase::update(float step)
{
	return eRunning;
}

void Chase::onExit() {

}