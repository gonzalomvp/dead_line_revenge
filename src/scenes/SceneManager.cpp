#include "common/stdafx.h"
#include "SceneManager.h"

#include "scenes/GameScene.h"
#include "scenes/MenuScene.h"

CSceneManager::~CSceneManager() {
	if (m_pCurrentScene) {
		DELETE(m_pCurrentScene);
	}
}

void CSceneManager::processInput() const {
	ASSERT(m_pCurrentScene);
	m_pCurrentScene->processInput();
}

void CSceneManager::run(float _fDeltaTime) const {
	ASSERT(m_pCurrentScene);
	m_pCurrentScene->run(_fDeltaTime);
}

void CSceneManager::render() const {
	ASSERT(m_pCurrentScene);
	m_pCurrentScene->render();
}

void CSceneManager::applyScene() {
	ASSERT(m_eWantedSceneType != IScene::EINVALID, "Not a valid Scene selected");

	IScene::EScene eCurrentSceneType = IScene::EINVALID;
	if (m_pCurrentScene) {
		eCurrentSceneType = m_pCurrentScene->getSceneType();
	}
	if (m_eWantedSceneType != eCurrentSceneType) {
		if (m_pCurrentScene) {
			DELETE(m_pCurrentScene);
		}
		switch (m_eWantedSceneType) {
			case IScene::EMENU:
				m_pCurrentScene = NEW(CMenuScene);
				m_pCurrentScene->init();
				break;
			case IScene::EGAME:
				m_pCurrentScene = NEW(CGameScene, m_uLevel);
				m_pCurrentScene->init();
				break;
		}	
	}
}

void CSceneManager::switchScene(IScene::EScene _eWantedSceneType, uint16_t _uLevel) {
	m_eWantedSceneType = _eWantedSceneType;
	m_uLevel           = _uLevel;
}