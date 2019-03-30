#include "common/stdafx.h"
#include "RotateAim.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/BehaviorTreeComponent.h"

void CRotateAim::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("fAngle"));
	m_fAngle = std::stof(behaviorElem->Attribute("fAngle"));
}

EStatus CRotateAim::onUpdate(float step) {
	Entity* self = getOwnerEntity();
	MessageGetAimDirection messageGetAimDirection;
	self->receiveMessage(&messageGetAimDirection);

	float angle = vangle(messageGetAimDirection.direction);
	angle += m_fAngle * step;

	MessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.direction = vunit(DEG2RAD(angle));
	self->receiveMessage(&messageSetAimDirection);

	return EStatus::ESuccess;
}