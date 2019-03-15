#include "common/stdafx.h"
#include "behavior.h"

#include "entities/components/behavior_tree/behavior_tree.h"

Behavior::SBehaviorInfo Behavior::s_aBehaviorInfo[] = {
#define REG_BEHAVIOR(Val, name) \
	{E##Val, name},
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR
};

Behavior::EType Behavior::getBehaviorTypeByName(const std::string& name) {
	EType etype = EType::EInvalid;
	int i = 0;
	while ((etype == EType::EInvalid) && (i < NUM_BEHAVIORS))
	{
		if (name == s_aBehaviorInfo[i].sName) {
			etype = s_aBehaviorInfo[i].eType;
		}
		i++;
	}
	return etype;
}

Entity* Behavior::getOwnerEntity() {
	ASSERT(mOwner);
	return mOwner->getOwner();
}

Status Behavior::tick(float step) {
	if (mStatus != eRunning) {
		onEnter();
	}
	mStatus = update(step);
	if (mStatus != eRunning) {
		onExit();
	}
	return mStatus;
}