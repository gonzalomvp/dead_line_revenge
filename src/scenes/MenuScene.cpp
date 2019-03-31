#include "common/stdafx.h"
#include "MenuScene.h"

#include "engine/GraphicEngine.h"
#include "gui/MenuManager.h"

void CMenuScene::init() {
	g_pMenuManager->activateMenu(CMenuManager::EMainMenu);
}

void CMenuScene::processInput() const {
	g_pInputManager->processInput();
}

void CMenuScene::run(float _fDeltaTime) const {
	g_pMenuManager->run(_fDeltaTime);
}

void CMenuScene::render() const {
	g_pGraphicEngine->render();
}