#pragma once

#include "scene.h"

class CGameScene : public IScene {
public:
	CGameScene(uint16_t _uLevel) : m_uLevel(_uLevel) {}
	virtual ~CGameScene() override;

	virtual void   init()                      override;
	virtual void   processInput()        const override;
	virtual void   run(float fDeltaTime) const override;
	virtual void   render()              const override;
	virtual EScene getSceneType()        const override { return EGAME; }
private:
	uint16_t m_uLevel;
};
