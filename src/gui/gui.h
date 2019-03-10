#pragma once

#include "../input/input_manager.h"

class Control : public IInputManager::IListener {
public:
	Control(const std::string& name, const vec2& pos, const vec2& size, bool isActive = true) : m_name(name), m_pos(pos), m_size(size), m_isActive(isActive) {}
	virtual ~Control() {}

	virtual void run       () {}
	virtual void activate  () { m_isActive = true;  }
	virtual void deactivate() { m_isActive = false; }

	virtual bool onEvent(const IInputManager::CEvent& event) = 0;
	
	bool        isActive() const { return m_isActive; }
	std::string getName () const { return m_name;     }
	vec2        getSize () const { return m_size;     }

protected:
	vec2        m_pos;
	vec2        m_size;
	std::string m_name;
	bool        m_isActive;
};