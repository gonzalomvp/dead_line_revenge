#include "common/stdafx.h"
#include "Aim.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/BehaviorTreeComponent.h"

void CAim::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sTo"));
	m_sBlackboardkey = behaviorElem->Attribute("sTo");
}

EStatus CAim::onUpdate(float step) {
	Entity* self = getOwnerEntity();
	Entity* player = g_pWorld->getPlayer();

	MessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.direction = vnorm(vsub(player->getPos(), self->getPos()));
	self->receiveMessage(&messageSetAimDirection);

	return EStatus::ESuccess;
}