#include "common/stdafx.h"
#include "bossIAComponent.h"

void BossIAComponent::init() {
	Component::init();
	loadFromXML(m_btFilename);
}