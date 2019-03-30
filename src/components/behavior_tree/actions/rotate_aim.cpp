#include "common/stdafx.h"
#include "rotate_aim.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/behavior_tree.h"

void CRotateAimAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("fAngle"));
	m_fAngle = std::stof(behaviorElem->Attribute("fAngle"));
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