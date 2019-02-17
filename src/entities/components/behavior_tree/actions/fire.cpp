#include "common/stdafx.h"
#include "fire.h"
#include "entities/entity.h"
#include "entities/components/behavior_tree/behavior_tree.h"
#include "entities/message.h"
#include "scenes/world.h"


Status Fire::update(float step) {

	Entity* self = mOwner->getCharacter();
	MessageFire messageFire;
	messageFire.isFiring = true;
	self->receiveMessage(&messageFire);

	return eSuccess;
}