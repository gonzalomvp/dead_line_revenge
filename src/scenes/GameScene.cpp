#include "common/stdafx.h"
#include "GameScene.h"

#include "engine/GraphicEngine.h"
#include "gui/MenuManager.h"
#include "scenes/World.h"

CWorld* g_pWorld;

void CGameScene::init() {
	ASSERT(g_pMenuManager && g_pWorld);
	g_pMenuManager->deactivateMenu();
	g_pWorld = NEW(CWorld);
	g_pWorld->init(m_uLevel);
}

CGameScene::~CGameScene() {
	DELETE(g_pWorld);
}

void CGameScene::processInput() const {
	ASSERT(g_pInputManager);
	g_pInputManager->processInput();
}

void CGameScene::run(float _fDeltaTime) const {
	ASSERT(g_pMenuManager && g_pWorld);
	g_pMenuManager->run(_fDeltaTime);
	g_pWorld->run(_fDeltaTime);
}

void CGameScene::render() const {
	ASSERT(g_pGraphicEngine);
	g_pGraphicEngine->render();
}