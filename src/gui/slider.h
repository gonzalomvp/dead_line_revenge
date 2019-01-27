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
		virtual void onValueChange(Slider* slider) = 0;
	};

	Slider(const std::string& name, const vec2& pos, const vec2& size, const char* spriteOn, const char* spriteOff, const std::string& text, float value);
	~Slider() {} //Implementar

	void init() {} //Implementar y mover a la clase Control
	virtual void activate();
	virtual void deactivate();
	virtual void run();
	
	virtual bool onEvent(const IInputManager::Event&);
	void addListener(IListener* listener) { m_listeners.push_back(listener); }

	virtual void onClick(Button* button);
	float getValue() { return m_value; }

private:

	void calculateValueFromPosition();
	void setValue(float value);

	Button* m_leftButton;
	Button* m_rightButton;
	Sprite* m_spriteBar;
	Sprite* m_spriteBall;
	std::vector<IListener*> m_listeners;
	bool m_ballPressed;
	bool m_barPressed;
	//std::string m_text;
	Text* m_gfxText;
	float m_value;
};