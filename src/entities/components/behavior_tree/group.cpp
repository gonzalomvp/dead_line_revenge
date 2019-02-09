#include "common/stdafx.h"
#include "group.h"

Group::~Group() {
	for (size_t i = 0; i < mChildren.size(); ++i) {
		delete mChildren[i];
	}
}