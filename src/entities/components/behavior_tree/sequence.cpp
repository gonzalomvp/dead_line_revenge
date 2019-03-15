#include "common/stdafx.h"
#include "sequence.h"

void CSequence::onEnter() {
	mCurrentChild = 0;
}

Status CSequence::update(float step) {
	while (true) {
		Status s = mChildren[mCurrentChild]->tick(step);
		if (s != eSuccess) {
			return s;
		}
		++mCurrentChild;
		if (mCurrentChild == mChildren.size()) {
			return eSuccess;
		}
	}
	return eInvalid;
}

