#pragma once

#include "components/Component.h"
#include "input/input_manager.h"

class CGfxEntity;
class CSprite;
class CText;

class CHUDComponent : public CComponent, public IInputManager::IListener {
public:
	CHUDComponent(Entity* owner)
	: CComponent(owner)
	, m_fMessageTimer(0.0f)
	, m_pLifeText(nullptr)
	, m_pScoreText(nullptr)
	, m_pAmmoText(nullptr)
	, m_pMessageText(nullptr)
	, m_pTargetSprite(nullptr)
	, m_pReloadSprite(nullptr)
	{}

	~CHUDComponent();

	// CComponent
	virtual void init()                             override;
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

	// IInputManager::IListener
	virtual bool onEvent(const IInputManager::CEvent& _event) override;

private:
	std::vector<CGfxEntity*> m_vGfxEntities;
	float                    m_fMessageTimer;
	CText*                   m_pLifeText;
	CText*                   m_pScoreText;
	CText*                   m_pAmmoText;
	CText*                   m_pMessageText;
	CSprite*                 m_pTargetSprite;
	CSprite*                 m_pReloadSprite;
};