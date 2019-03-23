#include "common/stdafx.h"
#include "change_weapon.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"

#include "components/behavior_tree/behavior_tree.h"

void CChangeWeaponAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 1, "CChangeWeaponAction must have 1 param");
	mWeaponName = vParams[0];
}

Status CChangeWeaponAction::update(float step) {

	Entity* self = getOwnerEntity();
	MessageWeaponChange messageWeaponChange;
	messageWeaponChange.eWeaponType = CWeaponComponent::getWeaponTypeByName(mWeaponName);
	self->receiveMessage(&messageWeaponChange);

	return eSuccess;
}