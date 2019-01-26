#pragma once

#include "gui.h"
#include <vector>

class Text;

//=============================================================================
// Label class
//=============================================================================
class Label : public Control {
public:
	Label(const std::string& name, const vec2& pos, const vec2& size, const std::string& text);
	~Label() {} //Implementar

	void init() {} //Implementar y mover a la clase Control
	virtual void activate();
	virtual void deactivate();
	virtual void run();

	virtual bool onEvent(const IInputManager::Event&) { return true; }

private:
	std::string              m_text;
	Text*                    m_gfxText;
};