#pragma once

#include "gui/controls/control.h"

#include <vector>

class CSprite;

class CCheckbox : public CControl {
public:
	class IListener {
	public:
		virtual void onClick(CCheckbox* _pCheckbox) = 0;
	};

	CCheckbox(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const char* _psCheckedImage, const char* _psUncheckedImage, bool _bIsChecked, bool _bIsActive = true);
	~CCheckbox();

	void addListener(IListener* _pListener) { m_listeners.push_back(_pListener); }
	bool isChecked() const { return m_bIsChecked; }

	// Control
	virtual void init      () override;
	virtual void activate  () override;
	virtual void deactivate() override;

	// IInputManager::IListener
	virtual bool onEvent (const IInputManager::CEvent& _event) override;

private:
	const char* m_psCheckedImage;
	const char* m_psUncheckedImage;
	bool        m_bIsPushed;
	bool        m_bIsChecked;
	CSprite*    m_pSpriteChecked;
	CSprite*    m_pSpriteUnchecked;
	
	std::vector<IListener*> m_listeners;
};