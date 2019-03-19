#pragma once

#include "gui/controls/control.h"
#include "gui/controls/button.h"
#include <vector>

class CSprite;
class CText;

class CSlider : public CControl, public CButton::IListener {
public:
	class IListener {
	public:
		virtual void onValueChange(CSlider* _pSlider) = 0;
	};

	CSlider(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const char* _psLeftNormalImage, const char* _psLeftPushImage, const char* _psRightNormalImage, const char* _psRightPushImage, const char* _psBarImage, const char* _psBallImage, float _fValue, float _fIncrement, bool _bIsActive = true);
	~CSlider();

	void addListener(IListener* _pListener) { m_vListeners.push_back(_pListener); }

	float getValue () const        { return m_fValue;    }
	void  setValue (float _fValue);

	// Control
	virtual void init      ()                                    override;
	virtual void run       (float _fDeltaTime)                   override;
	virtual void activate  ()                                    override;
	virtual void deactivate()                                    override;
	virtual bool onEvent   (const IInputManager::CEvent& _event) override;

	// CButton::IListener
	virtual void onClick(CButton* _pButton) override;

private:
	const char* m_psLeftNormalImage;
	const char* m_psLeftPushImage;
	const char* m_psRightNormalImage;
	const char* m_psRightPushImage;
	const char* m_psBarImage;
	const char* m_psBallImage;
	float       m_fValue;
	float       m_fIncrement;
	bool        m_bIsBallPushed;
	bool        m_bIsBarPushed;
	CButton*    m_pLeftButton;
	CButton*    m_pRightButton;
	CSprite*    m_pSpriteBar;
	CSprite*    m_pSpriteBall;
	CText*      m_pSliderText;

	std::vector<IListener*> m_vListeners;
};