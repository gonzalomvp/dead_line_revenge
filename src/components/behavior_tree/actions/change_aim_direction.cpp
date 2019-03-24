#include "common/stdafx.h"
#include "change_aim_direction.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/behavior_tree.h"


Status CChangeAimDirectionAction::update(float step) {
	Entity* self = getOwnerEntity();

	MessageSetAimDirection messageSetAimDirection;
	self->receiveMessage(&messageSetAimDirection);

	return eSuccess;
}