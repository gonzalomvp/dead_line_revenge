#pragma once

#include "scene.h"

class CSceneManager {
public:
	CSceneManager() : m_pCurrentScene(nullptr), m_eWantedSceneType(IScene::EINVALID), m_uLevel(0) {}
	~CSceneManager();

	void processInput()                  const;
	void run         (float _fDeltaTime) const;
	void render      ()                  const;
	void applyScene  ();
	void switchScene (IScene::EScene _eWantedSceneType, uint16_t _uLevel = 0);
private:
	IScene*        m_pCurrentScene;
	IScene::EScene m_eWantedSceneType;
	uint16_t       m_uLevel;
};