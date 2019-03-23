#include "common/stdafx.h"
#include "WeaponPickupComponent.h"

#include "entities/entities_factory.h"
#include "gui/string_manager.h"
#include "messages/message.h"
#include "scenes/world.h"

namespace {
	const std::string s_sPickupText = "LTEXT_GUI_PICKUP_MESSAGE";
	const float s_fFleeMinWallDistance = 20.0f;
}

void CWeaponPickupComponent::receiveMessage(Message* _pMessage) {
	Component::receiveMessage(_pMessage);
	if (!m_isActive)
		return;

	ASSERT(_pMessage && g_pWorld && g_pWorld->getPlayer() && g_pStringManager && g_pEntitiesFactory);

	if (MessageDestroy* pMessage = dynamic_cast<MessageDestroy*>(_pMessage)) {
		MessageWeaponChange msgWeapon;
		msgWeapon.eWeaponType = m_eWeaponType;
		g_pWorld->getPlayer()->receiveMessage(&msgWeapon);

		std::string sHudMessageText = g_pStringManager->getText(s_sPickupText);
		switch (m_eWeaponType) {
#define REG_WEAPON(val, name) \
			case ComponentWeapon::E##val: \
				sHudMessageText += g_pStringManager->getText("LTEXT_GUI_"#val"_MESSAGE"); \
				break;
#include "REG_WEAPONS.h"
#undef REG_WEAPON
		default:
			break;
		}
		Entity* pHudMessage = g_pEntitiesFactory->createHUDMessage(sHudMessageText, 100);
		g_pWorld->addEntity(pHudMessage);
	}
}