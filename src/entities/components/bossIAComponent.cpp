#include "../../common/stdafx.h"
#include "bossIAComponent.h"

void BossIAComponent::init() {
	Component::init();
	load(m_btFilename);
}