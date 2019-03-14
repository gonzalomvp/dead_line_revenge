#include "common/stdafx.h"
#include "rotate_aim.h"
#include "entities/entity.h"
#include "entities/message.h"
#include "scenes/world.h"
#include "entities/components/behavior_tree/behavior_tree.h"


Status RotateAim::update(float step) {
	Entity* self = getOwnerEntity();
	MessageGetAimDirection messageGetAimDirection;
	self->receiveMessage(&messageGetAimDirection);

	float angle = vangle(messageGetAimDirection.direction);
	angle += mAngle;

	MessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.direction = vunit(DEG2RAD(angle));
	self->receiveMessage(&messageSetAimDirection);

	return eSuccess;
}