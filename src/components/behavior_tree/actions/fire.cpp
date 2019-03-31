#include "common/stdafx.h"
#include "Fire.h"
#include "entities/Entity.h"
#include "components/BehaviorTreeComponent.h"
#include "messages/Message.h"
#include "scenes/World.h"


EStatus CFire::onUpdate(float step) {

	Entity* self = getOwnerEntity();
	TMessageFire messageFire;
	messageFire.isFiring = true;
	self->receiveMessage(&messageFire);

	return EStatus::ESuccess;
}