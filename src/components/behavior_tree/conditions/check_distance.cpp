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

	ASSERT(vParams.size() == 2, "CCheckDistanceCondition must have 2 param");
	mMinDistance = std::stoi(vParams[0]);
	m_bNegate = std::stoi(vParams[1]);
}

Status CCheckDistanceCondition::update(float step) {
	Entity* self = getOwnerEntity();
	Entity* player = g_pWorld->getPlayer();

	float dist2 = vdist2(self->getPos(), player->getPos());

	mOwner->getBlackboard().setValueFloat("distance", dist2);
	mOwner->getBlackboard().setValueEntity("player", player);

	bool bIsAtDistance = false;
	if (dist2 <= (mMinDistance * mMinDistance)) {
		bIsAtDistance = true;
	}

	if (m_bNegate) {
		bIsAtDistance = !bIsAtDistance;
	}

	if (bIsAtDistance) {
		return eSuccess;
	}

	return eFail;
}
