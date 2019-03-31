#include "common/stdafx.h"
#include "WeaponPickupComponent.h"

#include "entities/EntitiesFactory.h"
#include "gui/StringManager.h"
#include "messages/Message.h"
#include "scenes/World.h"

namespace {
	const std::string s_sPickupText = "LTEXT_GUI_PICKUP_MESSAGE";
	const float s_fFleeMinWallDistance = 20.0f;
}

void CWeaponPickupComponent::receiveMessage(TMessage* _pMessage) {
	CComponent::receiveMessage(_pMessage);
	if (!m_bIsActive)
		return;

	ASSERT(_pMessage && g_pWorld && g_pWorld->getPlayer() && g_pStringManager && g_pEntitiesFactory);

	if (TMessageDestroy* pMessage = dynamic_cast<TMessageDestroy*>(_pMessage)) {
		TMessageWeaponChange msgWeapon;
		msgWeapon.eWeaponType = m_eWeaponType;
		g_pWorld->getPlayer()->receiveMessage(&msgWeapon);

		std::string sHudTMessageText = g_pStringManager->getText(s_sPickupText);
		switch (m_eWeaponType) {
#define REG_WEAPON(val, name) \
			case CWeaponComponent::E##val: \
				sHudTMessageText += g_pStringManager->getText("LTEXT_GUI_"#val"_MESSAGE"); \
				break;
#include "REG_WEAPONS.h"
#undef REG_WEAPON
		default:
			break;
		}

		TMessageShowHUDTMessage msgShowHUDTMessage;
		msgShowHUDTMessage.message = sHudTMessageText;
		msgShowHUDTMessage.time = 1.5f;
		g_pWorld->getPlayer()->receiveMessage(&msgShowHUDTMessage);
	}
}