#include "common/stdafx.h"
#include "rotate_aim.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/behavior_tree.h"

void CRotateAimAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 1, "CRotateAimAction must have 1 param");
	m_fAngle = std::stof(vParams[0]);
}

Status CRotateAimAction::update(float step) {
	Entity* self = getOwnerEntity();
	MessageGetAimDirection messageGetAimDirection;
	self->receiveMessage(&messageGetAimDirection);

	float angle = vangle(messageGetAimDirection.direction);
	angle += m_fAngle * step;

	MessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.direction = vunit(DEG2RAD(angle));
	self->receiveMessage(&messageSetAimDirection);

	return eSuccess;
}