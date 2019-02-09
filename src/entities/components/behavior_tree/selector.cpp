#include "common/stdafx.h"
#include "selector.h"

void Selector::onEnter() {
	mCurrentChild = 0;
}

Status Selector::update(float step) {
	while (true) {
		Status s = mChildren[mCurrentChild]->tick(step);
		if (s != eFail) {
			return s;
		}
		++mCurrentChild;
		if (mCurrentChild == mChildren.size())
		{
			return eFail;
		}
	}
	return eInvalid;
}

