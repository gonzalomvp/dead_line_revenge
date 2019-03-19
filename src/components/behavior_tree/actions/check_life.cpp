#include "common/stdafx.h"
#include "check_life.h"
#include "entities/entity.h"
#include "entities/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/behavior_tree.h"

void CCheckLifeCondition::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 1, "CCheckLifeCondition must have 1 param");
	m_iLife = std::stoi(vParams[0]);
}

Status CCheckLifeCondition::update(float step) {
	Entity* self = getOwnerEntity();
	MessageGetLife messageGetLife;
	self->receiveMessage(&messageGetLife);

	if (messageGetLife.currentLife >= m_iLife) {
		return eSuccess;
	}

	return eFail;
}