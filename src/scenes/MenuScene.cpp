#include "common/stdafx.h"
#include "MenuScene.h"

#include "engine/GraphicEngine.h"
#include "gui/MenuManager.h"

void CMenuScene::init() {
	ASSERT(g_pMenuManager);
	g_pMenuManager->activateMenu(CMenuManager::EMainMenu);
}

void CMenuScene::processInput() const {
	ASSERT(g_pInputManager);
	g_pInputManager->processInput();
}

void CMenuScene::run(float _fDeltaTime) const {
	ASSERT(g_pMenuManager);
	g_pMenuManager->run(_fDeltaTime);
}

void CMenuScene::render() const {
	ASSERT(g_pGraphicEngine);
	g_pGraphicEngine->render();
}