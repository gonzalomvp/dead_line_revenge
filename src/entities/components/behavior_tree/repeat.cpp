#include "common/stdafx.h"
#include "repeat.h"

void Repeat::onEnter() {
	mCount = 0;
}

Status Repeat::update(float step) {
	ASSERT(mChildren.size() == 1, "Repeat decorators must only have one child behavior node");
	
	if (mCount < mNumRepeats) {
		Status s = mChildren[0]->tick(step);
		
		if (s == eFail) {
			return eFail;
		}
		else if (s == eSuccess) {
			++mCount;
		}
		return eRunning;
	}

	return eSuccess;
}

