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
		virtual void onClick(Button* button) = 0;
	};

	Button(const std::string& name, const vec2& pos, const vec2& size, bool isActive = true) : Control(name, pos, size, isActive) {}
	~Button();

	void init(const char* normalImage, const char* pushImage, const std::string& text, bool notifyHold = false, unsigned int holdTimer = 10);

	void addListener(IListener* listener) { m_listeners.push_back(listener); }

	// Control Interface
	virtual void activate  () override;
	virtual void deactivate() override;
	virtual void run       () override;
	virtual bool onEvent(const IInputManager::CEvent&) override;

private:
	Sprite*      m_spriteNormal;
	Sprite*      m_spritePush;
	std::string  m_text;
	Text*        m_buttonText;
	bool         m_isPushed;
	bool         m_isHold;
	unsigned int m_holdTime;
	unsigned int m_holdTimer;
	bool         m_notifyHold;

	std::vector<IListener*> m_listeners;
};