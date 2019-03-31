#include "common/stdafx.h"
#include "RotateAim.h"
#include "entities/Entity.h"
#include "messages/Message.h"
#include "scenes/World.h"
#include "components/BehaviorTreeComponent.h"

void CRotateAim::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("fAngle"));
	m_fAngle = std::stof(behaviorElem->Attribute("fAngle"));
}

EStatus CRotateAim::onUpdate(float step) {
	Entity* self = getOwnerEntity();
	TMessageGetAimDirection messageGetAimDirection;
	self->receiveMessage(&messageGetAimDirection);

	float angle = vangle(messageGetAimDirection.v2Dir);
	angle += DEG2RAD(m_fAngle) * step;

	TMessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.v2Dir = vunit(angle);
	self->receiveMessage(&messageSetAimDirection);

	return EStatus::ESuccess;
}