#include "common/stdafx.h"
#include "wait.h"
#include "entities/message.h"
#include "scenes/world.h"
#include "entities/components/behavior_tree/behavior_tree.h"

void Wait::onEnter() {
	mWaitTimer = 0;
}

Status Wait::update(float step) {
	if (mWaitTimer >= mWaitTime) {
		return eSuccess;
	}

	++mWaitTimer;

	return eRunning;
}