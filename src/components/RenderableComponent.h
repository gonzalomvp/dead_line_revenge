#pragma once

#include "components/Component.h"

class CSprite;

class CRenderableComponent : public CComponent {
public:
	CRenderableComponent(Entity* _pOwner, const std::string& _sTexture, float _fAngle, float _fAlpha, int _iPriority, bool _bAlignToMovement = false, bool _bAlignToAim = false, int _iHitTime = 0)
	: CComponent(_pOwner)
	, m_sTexture(_sTexture)
	, m_fAngle(_fAngle)
	, m_fAlpha(_fAlpha)
	, m_iPriority(_iPriority)
	, m_bAlignToMovement(_bAlignToMovement)
	, m_bAlignToAim(_bAlignToAim)
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
	bool        m_bAlignToMovement;
	bool        m_bAlignToAim;
	int         m_iHitTime;
	CSprite*    m_pSprite;

	//Timers
	int     m_iHitTimer;
};