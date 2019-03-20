#include "common/stdafx.h"
#include "check_distance.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/blackboard.h"

Status CheckDistance::update(float step) {
	Entity* self = getOwnerEntity();
	Entity* player = g_pWorld->getPlayer();
	MessageGetTransform messageSelfPos;
	self->receiveMessage(&messageSelfPos);
	MessageGetTransform messagePlayerPos;
	player->receiveMessage(&messagePlayerPos);

	float dist2 = vdist2(messageSelfPos.pos, messagePlayerPos.pos);

	mOwner->getBlackboard().setValueFloat("distance", dist2);
	mOwner->getBlackboard().setValueEntity("player", player);

	if (dist2 < (mMinDistance * mMinDistance)) {
		return eSuccess;
	}

	return eFail;
}
