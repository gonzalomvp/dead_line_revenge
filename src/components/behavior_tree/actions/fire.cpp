#include "common/stdafx.h"
#include "Fire.h"
#include "entities/entity.h"
#include "components/BehaviorTreeComponent.h"
#include "messages/message.h"
#include "scenes/world.h"


EStatus CFire::onUpdate(float step) {

	Entity* self = getOwnerEntity();
	MessageFire messageFire;
	messageFire.isFiring = true;
	self->receiveMessage(&messageFire);

	return EStatus::ESuccess;
}