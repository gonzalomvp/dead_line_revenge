#include "common/stdafx.h"
#include "change_speed.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "components/behavior_tree/behavior_tree.h"

void CChangeSpeedAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 1, "CChangeSpeedAction must have 1 param");
	m_fSpeed = std::stof(vParams[0]);
}

Status CChangeSpeedAction::update(float step) {
	Entity* self = getOwnerEntity();
	MessageSetMovementSpeed messageSetMovementSpeed;
	messageSetMovementSpeed.speed = m_fSpeed;
	self->receiveMessage(&messageSetMovementSpeed);

	return eSuccess;
}