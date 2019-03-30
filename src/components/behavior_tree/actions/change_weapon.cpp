#include "common/stdafx.h"
#include "change_weapon.h"
#include "entities/entity.h"
#include "entities/entities_factory.h"
#include "messages/message.h"
#include "scenes/world.h"

#include "components/behavior_tree/behavior_tree.h"

void CChangeWeaponAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sWeapon"));
	mWeaponName = behaviorElem->Attribute("sWeapon");
}

Status CChangeWeaponAction::update(float step) {

	Entity* self = getOwnerEntity();
	MessageWeaponChange messageWeaponChange;
	messageWeaponChange.eWeaponType = CEntitiesFactory::getWeaponTypeByName(mWeaponName);
	self->receiveMessage(&messageWeaponChange);

	return eSuccess;
}