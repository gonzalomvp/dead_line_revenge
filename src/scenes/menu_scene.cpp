#include "common/stdafx.h"
#include "menu_scene.h"

#include "globals.h"
#include "engine/graphics_engine.h"
#include "gui/menu.h"

void CMenuScene::init() {
	g_pMenuManager->activateMenu(MenuManager::EMainMenu);
}

void CMenuScene::processInput() const {
	g_pInputManager->processInput();
}

void CMenuScene::run(float _fDeltaTime) const {
	g_pMenuManager->run(_fDeltaTime);
}

void CMenuScene::render() const {
	g_pGraphicsEngine->render();
}