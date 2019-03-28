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
}

bool CCheckDistanceCondition::check(float step) {
	Entity* self = getOwnerEntity();
	Entity* player = g_pWorld->getPlayer();

	float dist2 = vdist2(self->getPos(), player->getPos());

	mOwner->getBlackboard().setValueFloat("distance", dist2);
	mOwner->getBlackboard().setValueEntity("player", player);

	return dist2 <= mMinDistance * mMinDistance;
}
