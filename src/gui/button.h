#pragma once

#include "gui.h"
#include <vector>

class Sprite;
class Text;

//=============================================================================
// Button class
//=============================================================================
class Button : public Control {
public:
	class IListener {
	public:
		virtual void onClick(Button* button) {}
		virtual void onHold(Button* button) {}
	};

	Button(const std::string& name, const vec2& pos, const vec2& size, const char* spriteOn, const char* spriteOff, const std::string& text);
	~Button() {} //Implementar

	void init() {} //Implementar y mover a la clase Control
	virtual void activate();
	virtual void deactivate();
	virtual void run();
	
	virtual bool onEvent(const IInputManager::Event&);
	void addListener(IListener* listener) { m_listeners.push_back(listener); }

private:
	Sprite* m_spriteOn;
	Sprite* m_spriteOff;
	std::vector<IListener*> m_listeners;
	bool m_pressed;
	std::string              m_text;
	Text*                    m_gfxText;
	unsigned int m_timer;
};