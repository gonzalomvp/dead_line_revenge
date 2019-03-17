#pragma once

#include "gui/controls/control.h"

#include <vector>

class Sprite;
class Text;

class CCheckbox : public CControl {
public:
	class IListener {
	public:
		virtual void onClick(CCheckbox* _pCheckbox) = 0;
	};

	CCheckbox(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, const char* _psCheckedImage, const char* _psUncheckedImage, bool _bIsChecked, bool _bIsActive = true);
	~CCheckbox();

	void addListener(IListener* listener) { m_listeners.push_back(listener); }
	bool isChecked() { return m_bIsChecked; }

	// Control
	virtual void init      ()                                    override;
	virtual void activate  ()                                    override;
	virtual void deactivate()                                    override;
	virtual bool onEvent   (const IInputManager::CEvent& _event) override;

private:
	const char* m_psCheckedImage;
	const char* m_psUncheckedImage;
	bool        m_bIsPushed;
	bool        m_bIsChecked;
	Sprite*     m_pSpriteChecked;
	Sprite*     m_pSpriteUnchecked;
	
	std::vector<IListener*> m_listeners;
};