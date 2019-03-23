#pragma once

#include "components/component.h"

class CRenderableComponent : public Component {
public:
	CRenderableComponent(Entity* _pOwner, const std::string& _sTexture, float _fAngle, float _fAlpha, int _iPriority, int _iHitTime = 0)
	: Component(_pOwner)
	, m_sTexture(_sTexture)
	, m_fAngle(_fAngle)
	, m_fAlpha(_fAlpha)
	, m_iPriority(_iPriority)
	, m_iHitTime(_iHitTime)
	, m_iHitTimer(0)
	{}

	~CRenderableComponent();

	// CComponent
	virtual void init()                             override;
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

private:
	std::string m_sTexture;
	float       m_fAngle;
	float       m_fAlpha;
	int         m_iPriority;
	int         m_iHitTime;
	CSprite*    m_pSprite;

	//Timers
	int     m_iHitTimer;
};