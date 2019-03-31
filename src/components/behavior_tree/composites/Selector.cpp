#include "common/stdafx.h"
#include "Selector.h"

void CSelector::onEnter() {
	mCurrentChild = 0;
}

CBehavior::EStatus CSelector::onUpdate(float step) {
	while (true) {
		EStatus s = mChildren[mCurrentChild]->run(step);
		if (s != EStatus::EFail) {
			return s;
		}
		++mCurrentChild;
		if (mCurrentChild == mChildren.size())
		{
			return EStatus::EFail;
		}
	}
	return EStatus::EInvalid;
}

