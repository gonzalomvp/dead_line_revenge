#include "common/stdafx.h"
#include "HUDMessageComponent.h"

#include "engine/graphics_engine.h"
#include "engine/text.h"
#include "entities/entity.h"
#include "gui/string_manager.h"
#include "scenes/world.h"

CHUDMessageComponent::~CHUDMessageComponent() {
	if (g_pGraphicsEngine) {
		g_pGraphicsEngine->removeGfxEntity(m_sMessage);
	}
	DELETE(m_sMessage);
}

void CHUDMessageComponent::init() {
	Component::init();

	ASSERT(m_owner && g_pGraphicsEngine && g_pStringManager);

	m_sMessage = NEW(CText, m_sMessageText, vmake((WORLD_WIDTH * 0.5f) - g_pStringManager->calculateTextHalfWidth(m_sMessageText), 20.0f), 1);
	g_pGraphicsEngine->addGfxEntity(m_sMessage);
}