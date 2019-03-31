#pragma once

#include "scenes/IScene.h"

class CMenuScene : public IScene {
public:
	CMenuScene() {}
	virtual ~CMenuScene() override {}

	virtual void   init()                      override;
	virtual void   processInput()        const override;
	virtual void   run(float fDeltaTime) const override;
	virtual void   render()              const override;
	virtual EScene getSceneType()        const override { return EMENU; }
};