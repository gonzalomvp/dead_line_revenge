#include "common/stdafx.h"
#include "check_distance.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/blackboard.h"

void CCheckDistanceCondition::init(TiXmlElement* behaviorElem) {
	CConditionNode::init(behaviorElem);

	ASSERT(behaviorElem->Attribute("fDistance"));
	mMinDistance = std::stoi(behaviorElem->Attribute("fDistance"));

	ASSERT(behaviorElem->Attribute("sBlackboardKey"));
	m_sBlackboardKey = behaviorElem->Attribute("sBlackboardKey");
}

bool CCheckDistanceCondition::check(float step) {
	Entity* self = getOwnerEntity();
	vec2 v2SelfPos = self->getPos();

	Entity* pOther;
	vec2 v2TargetPos = vmake(0.0f, 0.0f);

	bool bFound = mOwner->getBlackboard().getValueEntity(m_sBlackboardKey, pOther);
	if (bFound) {
		ASSERT(pOther);
		v2TargetPos = pOther->getPos();
	}
	else {
		bFound = mOwner->getBlackboard().getValueVec2(m_sBlackboardKey, v2TargetPos);
	}
	if (!bFound) {
		return false;
	}

	return vdist2(v2SelfPos, v2TargetPos) <= mMinDistance * mMinDistance;
}
