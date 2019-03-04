#include "common/stdafx.h"
#include "change_weapon.h"
#include "entities/entity.h"
#include "entities/entities_factory.h"
#include "entities/message.h"
#include "scenes/world.h"

#include "entities/components/behavior_tree/behavior_tree.h"


Status ChangeWeapon::update(float step) {

	Entity* self = mOwner->getCharacter();
	MessageWeaponChange messageWeaponChange;
	messageWeaponChange.weaponData = g_pEntitiesFactory->getWeaponDef(ComponentWeapon::getWeaponTypeByName(mWeaponName));
	self->receiveMessage(&messageWeaponChange);

	return eSuccess;
}