#include "common/stdafx.h"
#include "ChangeWeapon.h"
#include "entities/entity.h"
#include "entities/entities_factory.h"
#include "messages/message.h"
#include "scenes/world.h"

#include "components/BehaviorTreeComponent.h"

void CChangeWeapon::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sWeapon"));
	mWeaponName = behaviorElem->Attribute("sWeapon");
}

EStatus CChangeWeapon::onUpdate(float step) {

	Entity* self = getOwnerEntity();
	MessageWeaponChange messageWeaponChange;
	messageWeaponChange.eWeaponType = CEntitiesFactory::getWeaponTypeByName(mWeaponName);
	self->receiveMessage(&messageWeaponChange);

	return EStatus::ESuccess;
}