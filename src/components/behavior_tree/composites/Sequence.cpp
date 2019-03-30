#include "common/stdafx.h"
#include "Sequence.h"

void CSequence::onEnter() {
	mCurrentChild = 0;
}

EStatus CSequence::onUpdate(float step) {
	while (true) {
		EStatus s = mChildren[mCurrentChild]->run(step);
		if (s != EStatus::ESuccess) {
			return s;
		}
		++mCurrentChild;
		if (mCurrentChild == mChildren.size()) {
			return EStatus::ESuccess;
		}
	}
	return EStatus::EInvalid;
}

