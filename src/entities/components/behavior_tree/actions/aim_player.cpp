#include "common/stdafx.h"
#include "aim_player.h"
#include "entities/entity.h"
#include "entities/message.h"
#include "scenes/world.h"
#include "entities/components/behavior_tree/behavior_tree.h"


Status AimPlayer::update(float step) {
	Entity* self = mOwner->getCharacter();
	Entity* player = g_pWorld->getPlayer();
	MessageGetTransform messageSelfPos;
	self->receiveMessage(&messageSelfPos);
	MessageGetTransform messagePlayerPos;
	player->receiveMessage(&messagePlayerPos);

	MessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.direction = vnorm(vsub(messagePlayerPos.pos, messageSelfPos.pos));
	self->receiveMessage(&messageSetAimDirection);

	return eSuccess;
}