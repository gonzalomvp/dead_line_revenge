#include "common/stdafx.h"
#include "behavior.h"

#include "components/behavior_tree/behavior_tree.h"

CBehaviorNode::SBehaviorInfo CBehaviorNode::s_aBehaviorInfo[] = {
#define REG_BEHAVIOR(val, name) \
	{E##val, name},
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR
};

CBehaviorNode::EType CBehaviorNode::getBehaviorTypeByName(const std::string& name) {
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

Entity* CBehaviorNode::getOwnerEntity() {
	ASSERT(mOwner);
	return mOwner->getOwner();
}

Status CBehaviorNode::tick(float step) {
	if (mStatus != eRunning) {
		onEnter();
	}
	mStatus = update(step);
	if (mStatus != eRunning) {
		onExit();
	}
	return mStatus;
}