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

CBehavior::EType CBehavior::getBehaviorTypeByName(const std::string& _sName) {
	EType etype = EType::EUnknown;
	int i = 0;
	while ((etype == EType::EUnknown) && (i < NUM_BEHAVIORS))
	{
		if (_sName == s_aBehaviorInfo[i].sName) {
			etype = s_aBehaviorInfo[i].eType;
		}
		i++;
	}
	return etype;
}

CBehavior* CBehavior::createBehaviorFromXML(CBehaviorTreeComponent* _pOwnerComponent, TiXmlElement* _pBehaviorElem) {
	ASSERT(_pOwnerComponent && _pBehaviorElem);
	CBehavior* pBehavior = nullptr;

	std::string sTypeName = _pBehaviorElem->Value();
	EType eBehaviorType = CBehavior::getBehaviorTypeByName(sTypeName);

	switch (eBehaviorType) {
#define REG_BEHAVIOR(val) \
		case EType::E##val: \
			pBehavior = new C##val(_pOwnerComponent); \
			pBehavior->init(_pBehaviorElem); \
			break;
#include "REG_BEHAVIORS.h"
#undef REG_BEHAVIOR

	default:
		ASSERT(false, "Unknow Behavior: %s", sTypeName.c_str());
		break;
	}
	return pBehavior;
}

CBehavior::EStatus CBehavior::run(float _fDeltaTime) {
	if (m_eStatus != EStatus::ERunning) {
		onEnter();
	}
	m_eStatus = onUpdate(_fDeltaTime);
	if (m_eStatus != EStatus::ERunning) {
		onExit();
	}
	return m_eStatus;
}

CEntity* CBehavior::getOwnerEntity() const{
	ASSERT(m_pOwnerComponent);
	return m_pOwnerComponent->getOwner();
}
