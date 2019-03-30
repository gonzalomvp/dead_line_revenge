#include "common/stdafx.h"
#include "Parallel.h"

EStatus CParallel::onUpdate(float step) {
	EStatus returnStatus = EStatus::ESuccess;
	for (size_t i = 0; i < mChildren.size(); ++i) {
		EStatus s = mChildren[i]->run(step);
		if (s != EStatus::ESuccess) {
			returnStatus = s;
			if (s == EStatus::EFail) {
				break;
			}
		}
	}

	if (returnStatus == EStatus::EFail) {
		for (size_t i = 0; i < mChildren.size(); ++i) {
			if (mChildren[i]->getStatus() == EStatus::ERunning) {
				mChildren[i]->abort();
			}
		}
	}

	return returnStatus;
}
