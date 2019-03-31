#include "common/stdafx.h"
#include "ChangeWeapon.h"

#include "entities/Entity.h"
#include "entities/EntitiesFactory.h"
#include "messages/Message.h"

void CChangeWeapon::init(TiXmlElement* _pOwnerComponent) {
	CBehavior::init(_pOwnerComponent);
	ASSERT(_pOwnerComponent);

	ASSERT(_pOwnerComponent->Attribute("sWeapon"));
	m_sWeaponName = _pOwnerComponent->Attribute("sWeapon");
}

CBehavior::EStatus CChangeWeapon::onUpdate(float _fDeltaTime) {
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity);
	TMessageWeaponChange messageWeaponChange;
	messageWeaponChange.eWeaponType = CEntitiesFactory::getWeaponTypeByName(m_sWeaponName);
	pOwnerEntity->receiveMessage(&messageWeaponChange);

	return EStatus::ESuccess;
}