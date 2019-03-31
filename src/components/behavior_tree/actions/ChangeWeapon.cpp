#include "common/stdafx.h"
#include "ChangeWeapon.h"
#include "entities/Entity.h"
#include "entities/EntitiesFactory.h"
#include "messages/Message.h"
#include "scenes/World.h"

#include "components/BehaviorTreeComponent.h"

void CChangeWeapon::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sWeapon"));
	mWeaponName = behaviorElem->Attribute("sWeapon");
}

EStatus CChangeWeapon::onUpdate(float step) {

	Entity* self = getOwnerEntity();
	TMessageWeaponChange messageWeaponChange;
	messageWeaponChange.eWeaponType = CEntitiesFactory::getWeaponTypeByName(mWeaponName);
	self->receiveMessage(&messageWeaponChange);

	return EStatus::ESuccess;
}