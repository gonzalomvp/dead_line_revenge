#pragma once

#include "components/Component.h"

class CSprite;

class CRenderableComponent : public CComponent {
public:
	CRenderableComponent(CEntity* _pOwner, const std::string& _sTexture, float _fAngle, float _fAlpha, int _iPriority, bool _bAlignToMovement = false, bool _bAlignToAim = false, float _fHitTime = 0.0f)
	: CComponent(_pOwner)
	, m_sTexture(_sTexture)
	, m_fAngle(_fAngle)
	, m_fAlpha(_fAlpha)
	, m_iPriority(_iPriority)
	, m_bAlignToMovement(_bAlignToMovement)
	, m_bAlignToAim(_bAlignToAim)
	, m_fHitTime(_fHitTime)
	, m_fHitTimer(0.0f)
	, m_pSprite(nullptr)
	{}

	~CRenderableComponent();

	// CComponent
	virtual void init()                             override;
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(TMessage* _pMessage) override;

private:
	std::string m_sTexture;
	float       m_fAngle;
	float       m_fAlpha;
	int         m_iPriority;
	bool        m_bAlignToMovement;
	bool        m_bAlignToAim;
	float       m_fHitTime;
	CSprite*    m_pSprite;

	//Timers
	float       m_fHitTimer;
};