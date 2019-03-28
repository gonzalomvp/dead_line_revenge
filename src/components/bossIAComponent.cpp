#include "common/stdafx.h"
#include "bossIAComponent.h"
#include "scenes/world.h"

void BossIAComponent::init() {
	Component::init();
	loadFromXML(m_btFilename);

	if (g_pWorld->getPlayer()) {
		getBlackboard().setValueEntity("player", g_pWorld->getPlayer());
	}
	
}