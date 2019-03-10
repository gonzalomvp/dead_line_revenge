#pragma once

#include "control.h"
#include <vector>

class Text;

//=============================================================================
// Label class
//=============================================================================
class Label : public CControl {
public:
	Label(const std::string& name, const vec2& pos, const vec2& size, bool isActive = true) : CControl(name, pos, size, isActive) {}
	~Label();

	void init(const std::string& text);

	// Control Interface
	virtual void activate  () override;
	virtual void deactivate() override;
	virtual void run       (float _fDeltaTime) override;
	virtual bool onEvent(const IInputManager::CEvent&) override { return true; }

private:
	std::string m_text;
	Text*   m_labelText;
};