#include "common/stdafx.h"
#include "ChangeSpeed.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "components/BehaviorTreeComponent.h"

void CChangeSpeed::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("fSpeed"));
	m_fSpeed = std::stof(behaviorElem->Attribute("fSpeed"));
}

EStatus CChangeSpeed::onUpdate(float step) {
	Entity* self = getOwnerEntity();
	MessageSetMovementSpeed messageSetMovementSpeed;
	messageSetMovementSpeed.speed = m_fSpeed;
	self->receiveMessage(&messageSetMovementSpeed);

	return EStatus::ESuccess;
}