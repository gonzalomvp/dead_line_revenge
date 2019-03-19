#include "common/stdafx.h"
#include "parallel.h"

Status CParallelNode::update(float step) {
	Status returnStatus = eSuccess;
	for (size_t i = 0; i < mChildren.size(); ++i) {
		Status s = mChildren[i]->tick(step);
		if (s != eSuccess) {
			returnStatus = s;
			if (s == eFail) {
				break;
			}
		}
	}

	if (returnStatus == eFail) {
		for (size_t i = 0; i < mChildren.size(); ++i) {
			if (mChildren[i]->getStatus() == eRunning) {
				mChildren[i]->abort();
			}
		}
	}

	return returnStatus;
}
