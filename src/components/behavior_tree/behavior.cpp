#include "common/stdafx.h"
#include "Behavior.h"

#include "components/BehaviorTreeComponent.h"

// Composites
#include "components/behavior_tree/composites/Parallel.h"
#include "components/behavior_tree/composites/Selector.h"
#include "components/behavior_tree/composites/Sequence.h"

// Decorators
#include "components/behavior_tree/decorators/Repeat.h"

// Conditions
#include "components/behavior_tree/conditions/CheckIsAtDistance.h"
#include "components/behavior_tree/conditions/CheckLife.h"

// Actions
#include "components/behavior_tree/actions/Aim.h"
#include "components/behavior_tree/actions/CalculateFleePosition.h"
#include "components/behavior_tree/actions/CalculateRandomPosition.h"
#include "components/behavior_tree/actions/ChangeSpeed.h"
#include "components/behavior_tree/actions/ChangeSprite.h"
#include "components/behavior_tree/actions/ChangeWeapon.h"
#include "components/behavior_tree/actions/Fire.h"
#include "components/behavior_tree/actions/GoToBlackboardPosition.h"
#include "components/behavior_tree/actions/RotateAim.h"
#include "components/behavior_tree/actions/Wait.h"

CBehavior::SBehaviorInfo CBehavior::s_aBehaviorInfo[] = {
#define REG_BEHAVIOR(val) \
	{E##val, #val},
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR
};

CBehavior::EType CBehavior::getBehaviorTypeByName(const std::string& name) {
	EType etype = EType::EUnknown;
	int i = 0;
	while ((etype == EType::EUnknown) && (i < NUM_BEHAVIORS))
	{
		if (name == s_aBehaviorInfo[i].sName) {
			etype = s_aBehaviorInfo[i].eType;
		}
		i++;
	}
	return etype;
}

CEntity* CBehavior::getOwnerEntity() {
	ASSERT(m_pOwnerComponent);
	return m_pOwnerComponent->getOwner();
}

CBehavior::EStatus CBehavior::run(float step) {
	if (m_eStatus != EStatus::ERunning) {
		onEnter();
	}
	m_eStatus = onUpdate(step);
	if (m_eStatus != EStatus::ERunning) {
		onExit();
	}
	return m_eStatus;
}

CBehavior* CBehavior::createBehaviorFromXML(CBehaviorTreeComponent* _pOwnerComponent, TiXmlElement* _pBehaviorElem) {
	ASSERT(_pOwnerComponent && _pBehaviorElem);
	CBehavior* behavior = nullptr;

	std::string type = _pBehaviorElem->Value();
	EType eBehaviorType = CBehavior::getBehaviorTypeByName(type);

	switch (eBehaviorType) {
#define REG_BEHAVIOR(val) \
		case EType::E##val: \
			behavior = new C##val(_pOwnerComponent); \
			behavior->init(_pBehaviorElem); \
			break;
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR

		default:
			ASSERT(false, "Invalid Behavior type: %s", type.c_str());
			break;
	}

	return behavior;
}