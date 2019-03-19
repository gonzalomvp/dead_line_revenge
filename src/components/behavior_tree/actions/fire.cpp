#include "common/stdafx.h"
#include "fire.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "entities/message.h"
#include "scenes/world.h"


Status CFireAction::update(float step) {

	Entity* self = getOwnerEntity();
	MessageFire messageFire;
	messageFire.isFiring = true;
	self->receiveMessage(&messageFire);

	return eSuccess;
}