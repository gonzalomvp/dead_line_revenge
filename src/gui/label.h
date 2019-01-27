#pragma once

#include "gui.h"
#include <vector>

class Text;

//=============================================================================
// Label class
//=============================================================================
class Label : public Control {
public:
	Label(const std::string& name, const vec2& pos, const vec2& size, bool isActive = true) : Control(name, pos, size, isActive) {}
	~Label();

	void init(const std::string& text);

	virtual void activate  () override;
	virtual void deactivate() override;
	virtual void run       () override;

	virtual bool onEvent(const IInputManager::Event&) { return true; }

private:
	std::string m_text;
	Text*       m_labelText;
};