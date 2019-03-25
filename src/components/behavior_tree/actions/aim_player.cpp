#include "common/stdafx.h"
#include "aim_player.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/behavior_tree.h"


Status CAimToPlayerAction::update(float step) {
	Entity* self = getOwnerEntity();
	Entity* player = g_pWorld->getPlayer();

	MessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.direction = vnorm(vsub(player->getPos(), self->getPos()));
	self->receiveMessage(&messageSetAimDirection);

	return eSuccess;
}