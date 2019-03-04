#include "common/stdafx.h"
#include "check_life.h"
#include "entities/entity.h"
#include "entities/message.h"
#include "scenes/world.h"
#include "entities/components/behavior_tree/behavior_tree.h"


Status CheckLife::update(float step) {
	Entity* self = mOwner->getCharacter();
	MessageGetLife messageGetLife;
	self->receiveMessage(&messageGetLife);

	if (messageGetLife.currentLife >= mLife) {
		return eSuccess;
	}

	return eFail;
}