#include "common/stdafx.h"
#include "check_distance.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/blackboard.h"

void CCheckDistanceCondition::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 1, "CCheckDistanceCondition must have 1 param");
	mMinDistance = std::stoi(vParams[0]);
}

Status CCheckDistanceCondition::update(float step) {
	Entity* self = getOwnerEntity();
	Entity* player = g_pWorld->getPlayer();

	float dist2 = vdist2(self->getPos(), player->getPos());

	mOwner->getBlackboard().setValueFloat("distance", dist2);
	mOwner->getBlackboard().setValueEntity("player", player);

	if (dist2 < (mMinDistance * mMinDistance)) {
		return eSuccess;
	}

	return eFail;
}
