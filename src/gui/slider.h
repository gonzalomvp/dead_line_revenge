#pragma once

#include "button.h"
#include "gui.h"

#include <vector>

class Sprite;
class Text;

//=============================================================================
// Button class
//=============================================================================
class Slider : public Control, Button::IListener {
public:
	class IListener {
	public:
		virtual void onValueChange(ptr<Slider> slider) = 0;
	};

	Slider(const std::string& name, const vec2& pos, const vec2& size, bool isActive = true) : Control(name, pos, size, isActive) {}
	~Slider();

	void init(const char* spriteLeftNormal, const char* spriteLeftPush, const char* spriteRightNormal, const char* spriteRightPush, const char* spriteBar, const char* spriteBall, float value);

	void  addListener(ptr<IListener> listener) { m_listeners.push_back(listener); }
	float getValue()                       { return m_value;                  }

	// Control Interface
	virtual void activate()   override;
	virtual void deactivate() override;
	virtual void run()        override;
	virtual bool onEvent(const IInputManager::Event&) override;

	// Button::IListener Interface
	virtual void onClick(ptr<Button> button) override;

private:
	void calculateValueFromPosition();
	void setValue(float value);

	ptr<Button> m_leftButton;
	ptr<Button> m_rightButton;
	ptr<Sprite> m_spriteBar;
	ptr<Sprite> m_spriteBall;
	ptr<Text>   m_sliderText;
	bool        m_isBallPushed;
	bool        m_isBarPushed;
	float       m_value;

	std::vector<ptr<IListener>> m_listeners;
};