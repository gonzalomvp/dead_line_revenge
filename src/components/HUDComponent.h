#pragma once

#include "components/Component.h"
#include "input/IInputManager.h"

class CGraphicEntity;
class CSprite;
class CText;

class CHUDComponent : public CComponent, public IInputManager::IListener {
public:
	CHUDComponent(CEntity* _pOwner)
	: CComponent(_pOwner)
	, m_fTMessageTimer(0.0f)
	, m_pLifeText(nullptr)
	, m_pScoreText(nullptr)
	, m_pAmmoText(nullptr)
	, m_pMessageText(nullptr)
	, m_pTargetSprite(nullptr)
	, m_pReloadSprite(nullptr)
	{}

	~CHUDComponent();

	// CComponent
	virtual void init()                              override;
	virtual void run(float _fDeltaTime)              override;
	virtual void receiveMessage(TMessage* _pMessage) override;

	// IInputManager::IListener
	virtual bool onEvent(const IInputManager::CEvent& _event) override;

private:
	std::vector<CGraphicEntity*> m_vGfxEntities;
	float                        m_fTMessageTimer;
	CText*                       m_pLifeText;
	CText*                       m_pScoreText;
	CText*                       m_pAmmoText;
	CText*                       m_pMessageText;
	CSprite*                     m_pTargetSprite;
	CSprite*                     m_pReloadSprite;
};