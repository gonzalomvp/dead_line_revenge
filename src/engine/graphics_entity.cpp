#include "common/stdafx.h"
#include "graphics_entitiy.h"

#include "engine/graphics_engine.h"

CGfxEntity::~CGfxEntity() {
	if (g_pGraphicsEngine) {
		g_pGraphicsEngine->removeGfxEntity(this);
	}
}