#include "common/stdafx.h"
#include "behavior.h"

#include "entities/components/behavior_tree/behavior_tree.h"

Entity* Behavior::getOwnerEntity() {
	ASSERT(mOwner);
	return mOwner->getOwner();
}

Status Behavior::tick(float step) {
	if (mStatus != eRunning) {
		onEnter();
	}
	mStatus = update(step);
	if (mStatus != eRunning) {
		onExit();
	}
	return mStatus;
}