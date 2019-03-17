#pragma once

#include "gui/controls/control.h"

#include <vector>

class Sprite;
class Text;

class CButton : public CControl {
public:
	class IListener {
	public:
		virtual void onClick(CButton* _pButton) = 0;
	};

	CButton(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const char* _psNormalImage, const char* _psPushImage, const std::string& _sText, float _fHoldTime = 0.0f, bool _bIsActive = true);
	~CButton();

	void addListener(IListener* _pListener) { m_vListeners.push_back(_pListener); }

	// Control
	virtual void init      ()                                    override;
	virtual void run       (float _fDeltaTime)                   override;
	virtual void activate  ()                                    override;
	virtual void deactivate()                                    override;
	virtual bool onEvent   (const IInputManager::CEvent& _event) override;

private:
	const char*  m_psNormalImage;
	const char*  m_psPushImage;
	std::string  m_sText;
	bool         m_bIsPushed;
	bool         m_bIsHold;
	float        m_fHoldTime;
	float        m_fHoldTimer;
	Sprite*      m_pSpriteNormal;
	Sprite*      m_pSpritePush;
	Text*        m_pButtonText;

	std::vector<IListener*> m_vListeners;
};