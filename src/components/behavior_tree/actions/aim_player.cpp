#include "common/stdafx.h"
#include "aim_player.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/behavior_tree.h"

void CAimTo::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sTo"));
	m_sBlackboardkey = behaviorElem->Attribute("sTo");
}

Status CAimTo::update(float step) {
	Entity* self = getOwnerEntity();
	Entity* player = g_pWorld->getPlayer();

	MessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.direction = vnorm(vsub(player->getPos(), self->getPos()));
	self->receiveMessage(&messageSetAimDirection);

	return eSuccess;
}