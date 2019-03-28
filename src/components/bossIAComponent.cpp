#include "common/stdafx.h"
#include "bossIAComponent.h"
#include "scenes/world.h"

void BossIAComponent::init() {
	Component::init();
	loadFromXML(m_btFilename);

	getBlackboard().setValueEntity("player", g_pWorld->getPlayer());
}