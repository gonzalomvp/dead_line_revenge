#include "common/stdafx.h"
#include "change_speed.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "components/behavior_tree/behavior_tree.h"

void CChangeSpeedAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("fSpeed"));
	m_fSpeed = std::stof(behaviorElem->Attribute("fSpeed"));
}

Status CChangeSpeedAction::update(float step) {
	Entity* self = getOwnerEntity();
	MessageSetMovementSpeed messageSetMovementSpeed;
	messageSetMovementSpeed.speed = m_fSpeed;
	self->receiveMessage(&messageSetMovementSpeed);

	return eSuccess;
}