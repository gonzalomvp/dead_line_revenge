#include "common/stdafx.h"
#include "sequence.h"

void CSequenceNode::onEnter() {
	mCurrentChild = 0;
}

Status CSequenceNode::update(float step) {
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

