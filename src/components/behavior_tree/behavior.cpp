#include "common/stdafx.h"
#include "Behavior.h"

#include "components/BehaviorTreeComponent.h"

CBehavior::SBehaviorInfo CBehavior::s_aBehaviorInfo[] = {
#define REG_BEHAVIOR(val) \
	{E##val, #val},
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR
};

CBehavior::EType CBehavior::getBehaviorTypeByName(const std::string& name) {
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

Entity* CBehavior::getOwnerEntity() {
	ASSERT(mOwner);
	return mOwner->getOwner();
}

EStatus CBehavior::run(float step) {
	if (mStatus != EStatus::ERunning) {
		onEnter();
	}
	mStatus = onUpdate(step);
	if (mStatus != EStatus::ERunning) {
		onExit();
	}
	return mStatus;
}