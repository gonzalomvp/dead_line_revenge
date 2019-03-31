#include "common/stdafx.h"
#include "CheckIsAtDistance.h"
#include "entities/Entity.h"
#include "components/BehaviorTreeComponent.h"
#include "messages/Message.h"
#include "scenes/World.h"
#include "components/behavior_tree/Blackboard.h"

void CCheckIsAtDistance::init(TiXmlElement* behaviorElem) {
	CCondition::init(behaviorElem);

	ASSERT(behaviorElem->Attribute("fDistance"));
	m_fDistance = std::stof(behaviorElem->Attribute("fDistance"));

	ASSERT(behaviorElem->Attribute("sPosition"));
	m_sBlackboardKey = behaviorElem->Attribute("sPosition");
}

bool CCheckIsAtDistance::check(float step) {
	CEntity* self = getOwnerEntity();
	vec2 v2SelfPos = self->getPos();

	CEntity* pOther;
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
