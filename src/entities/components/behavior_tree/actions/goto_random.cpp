#include "common/stdafx.h"
#include "goto_random.h"
#include "entities/entity.h"
#include "entities/components/behavior_tree/behavior_tree.h"
#include "entities/message.h"
#include "scenes/world.h"

void GoToRandomPosition::onEnter() {
	mTargetPos = vmake(CORE_FRand(0.0, WORLD_WIDTH), CORE_FRand(80, WORLD_HEIGHT - 80));
}

Status GoToRandomPosition::update(float step) {

	Entity* self = mOwner->getCharacter();

	MessageGetTransform messageSelfPos;
	self->receiveMessage(&messageSelfPos);
	vec2 direction = vsub(mTargetPos, messageSelfPos.pos);

	if (vlen2(direction) <= 20 * 20)
	{
		return eSuccess;
	}

	MessageSetTransform msgSetTransform;
	msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(direction), 2));
	msgSetTransform.size = messageSelfPos.size;
	self->receiveMessage(&msgSetTransform);

	return eRunning;
}