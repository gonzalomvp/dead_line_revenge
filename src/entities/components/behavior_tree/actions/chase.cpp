#include "common/stdafx.h"
#include "chase.h"

#include "entities/entity.h"
#include "entities/components/behavior_tree/behavior_tree.h"

Chase::Chase(BehaviorTree* owner, float arriveDistance, float lostDistance) : Behavior(owner), mArriveDistance(arriveDistance), mLostDistance(lostDistance) {

}

Chase::~Chase() {

	delete mPursueSteering;
	delete mAlignToMovementSteering;
}

void Chase::onEnter() {

}

Status Chase::update(float step)
{
	return eRunning;
}

void Chase::onExit() {

}