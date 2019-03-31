#include "common/stdafx.h"
#include "CheckIsAtDistance.h"
#include "entities/Entity.h"
#include "components/BehaviorTreeComponent.h"
#include "messages/Message.h"
#include "scenes/World.h"
#include "components/behavior_tree/Blackboard.h"

void CCheckIsAtDistance::init(TiXmlElement* _pBehaviorElem) {
	CCondition::init(_pBehaviorElem);

	ASSERT(_pBehaviorElem);

	ASSERT(_pBehaviorElem->Attribute("fDistance"));
	m_fDistance = std::stof(_pBehaviorElem->Attribute("fDistance"));

	ASSERT(_pBehaviorElem->Attribute("sPosition"));
	m_sBlackboardKey = _pBehaviorElem->Attribute("sPosition");
}

bool CCheckIsAtDistance::check(float _fDeltaTime) {
	CEntity* pOwnerEntity = getOwnerEntity();

	ASSERT(pOwnerEntity && m_pOwnerComponent);

	vec2 v2SelfPos = pOwnerEntity->getPos();

	CEntity* pOther = nullptr;
	vec2 v2TargetPos = vmake(0.0f, 0.0f);

	bool bFound = m_pOwnerComponent->getBlackboard().getValueEntity(m_sBlackboardKey, pOther);
	if (bFound) {
		ASSERT(pOther);
		v2TargetPos = pOther->getPos();
	}
	else {
		bFound = m_pOwnerComponent->getBlackboard().getValueVec2(m_sBlackboardKey, v2TargetPos);
	}
	if (!bFound) {
		return false;
	}

	return vdist2(v2SelfPos, v2TargetPos) <= m_fDistance * m_fDistance;
}
