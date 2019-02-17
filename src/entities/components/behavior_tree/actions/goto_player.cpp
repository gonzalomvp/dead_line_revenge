#include "common/stdafx.h"
#include "goto_player.h"
#include "entities/entity.h"
#include "entities/components/behavior_tree/behavior_tree.h"
#include "entities/message.h"
#include "scenes/world.h"

void GoToPlayerPosition::onEnter() {
	Entity* player = g_world->getPlayer();
	MessageGetTransform messagePlayerPos;
	player->receiveMessage(&messagePlayerPos);
	mTargetPos = messagePlayerPos.pos;
}

Status GoToPlayerPosition::update(float step) {

	Entity* self = mOwner->getCharacter();

	MessageGetTransform messageSelfPos;
	self->receiveMessage(&messageSelfPos);

	vec2 direction = vsub(mTargetPos, messageSelfPos.pos);

	if (vlen2(direction) <= 20 * 20)
	{
		return eSuccess;
	}

	MessageSetTransform msgSetTransform;
	msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(direction), mSpeed));
	msgSetTransform.size = messageSelfPos.size;
	self->receiveMessage(&msgSetTransform);

	return eRunning;
}