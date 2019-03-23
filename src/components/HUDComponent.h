#pragma once

#include "components/component.h"
#include "input/input_manager.h"

class CHUDComponent : public Component, public IInputManager::IListener {
public:
	CHUDComponent(Entity* owner)
	: Component(owner)
	, m_pLifeText(nullptr)
	, m_pScoreText(nullptr)
	, m_pAmmoText(nullptr)
	, m_pTargetSprite(nullptr)
	, m_pReloadSprite(nullptr)
	{}

	~CHUDComponent();

	// CComponent
	virtual void init()                 override;
	virtual void run(float _fDeltaTime) override;

	// IInputManager::IListener
	virtual bool onEvent(const IInputManager::CEvent& _event) override;

private:
	std::vector<CGfxEntity*> m_vGfxEntities;
	CText*                   m_pLifeText;
	CText*                   m_pScoreText;
	CText*                   m_pAmmoText;
	CSprite*                 m_pTargetSprite;
	CSprite*                 m_pReloadSprite;
	
};