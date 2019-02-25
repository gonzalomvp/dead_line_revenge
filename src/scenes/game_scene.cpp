#include "common/stdafx.h"
#include "game_scene.h"

#include "globals.h"
#include "engine/graphics_engine.h"
#include "gui/menu.h"
#include "scenes/world.h"

CWorld* g_pWorld;

void CGameScene::init() {
	g_pMenuManager->deactivateMenu();
	g_pWorld = NEW(CWorld, m_uLevel);
	g_pWorld->init();
	g_pWorld->loadLevel();
}

CGameScene::~CGameScene() {
	DELETE(g_pWorld);
}

void CGameScene::processInput() const {
	g_pInputManager->processInput();
}

void CGameScene::run(float _fDeltaTime) const {
	g_pMenuManager->run(_fDeltaTime);
	g_pWorld->run(_fDeltaTime);
}

void CGameScene::render() const {
	g_pGraphicsEngine->render();
}