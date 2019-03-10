#pragma once

#include "control.h"
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

	Checkbox(const std::string& name, const vec2& pos, const vec2& size, bool isActive = true) : Control(name, pos, size, isActive) {}
	~Checkbox();

	void init(const char* spriteChecked, const char* spriteUnchecked, bool isChecked);

	void addListener(IListener* listener) { m_listeners.push_back(listener); }
	bool isChecked() { return m_isChecked; }

	// Control Interface
	virtual void activate  () override;
	virtual void deactivate() override;
	virtual bool onEvent(const IInputManager::CEvent&) override;

private:
	Sprite* m_spriteChecked;
	Sprite* m_spriteUnchecked;
	
	bool m_isPushed;
	bool m_isChecked;

	std::vector<IListener*> m_listeners;
};