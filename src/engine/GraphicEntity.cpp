#include "common/stdafx.h"
#include "GraphicEntity.h"

#include "engine/GraphicEngine.h"

CGraphicEntity::~CGraphicEntity() {
	if (g_pGraphicEngine) {
		g_pGraphicEngine->removeGfxEntity(this);
	}
}