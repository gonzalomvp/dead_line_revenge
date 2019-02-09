#include "common/stdafx.h"
#include "behavior.h"

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