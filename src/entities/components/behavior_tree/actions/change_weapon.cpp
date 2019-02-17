#include "common/stdafx.h"
#include "change_weapon.h"
#include "entities/message.h"
#include "scenes/world.h"

#include "entities/components/behavior_tree/behavior_tree.h"


Status ChangeWeapon::update(float step) {

	Entity* self = mOwner->getCharacter();
	MessageWeaponChange messageWeaponChange;
	messageWeaponChange.weaponData = g_world->m_weaponData[static_cast<ComponentWeapon::TWeapon>(mWeaponID)];
	self->receiveMessage(&messageWeaponChange);

	return eSuccess;
}