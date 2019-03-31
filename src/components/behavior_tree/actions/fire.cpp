#include "common/stdafx.h"
#include "Fire.h"

#include "entities/Entity.h"
#include "messages/Message.h"


CBehavior::EStatus CFire::onUpdate(float _fDeltaTime) {
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity);
	TMessageFire messageFire;
	messageFire.bIsFiring = true;
	pOwnerEntity->receiveMessage(&messageFire);

	return EStatus::ESuccess;
}