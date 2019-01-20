#pragma once

#include "gui.h"
#include <vector>

class Sprite;
class Text;

//=============================================================================
// Checkbox class
//=============================================================================
class Checkbox : public Control {
public:
	class IListener {
	public:
		virtual void onClick(Checkbox* button) = 0;
	};

	Checkbox(const std::string& name, const vec2& pos, const vec2& size, const char* spriteOn, const char* spriteOff, const std::string& text);
	~Checkbox() {} //Implementar

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
	bool m_checked;
	//std::string              m_text;
	//Text*                    m_gfxText;
};