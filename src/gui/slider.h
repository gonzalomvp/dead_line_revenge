#pragma once

#include "button.h"
#include "control.h"

#include <vector>

class Sprite;
class Text;

//=============================================================================
// Button class
//=============================================================================
class Slider : public CControl, Button::IListener {
public:
	class IListener {
	public:
		virtual void onValueChange(Slider* slider) = 0;
	};

	Slider(const std::string& name, const vec2& pos, const vec2& size, bool isActive = true) : CControl(name, pos, size, isActive) {}
	~Slider();

	void init(const char* spriteLeftNormal, const char* spriteLeftPush, const char* spriteRightNormal, const char* spriteRightPush, const char* spriteBar, const char* spriteBall, float value);

	void  addListener(IListener* listener) { m_listeners.push_back(listener); }
	float getValue()                           { return m_value;                  }

	// Control Interface
	virtual void activate()   override;
	virtual void deactivate() override;
	virtual void run(float _fDeltaTime)        override;
	virtual bool onEvent(const IInputManager::CEvent&) override;

	// Button::IListener Interface
	virtual void onClick(Button* button) override;

private:
	void calculateValueFromPosition();
	void setValue(float value);

	Button* m_leftButton;
	Button* m_rightButton;
	Sprite* m_spriteBar;
	Sprite* m_spriteBall;
	Text*   m_sliderText;
	bool        m_isBallPushed;
	bool        m_isBarPushed;
	float       m_value;

	std::vector<IListener*> m_listeners;
};