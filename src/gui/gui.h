#pragma once

#include "../input/input_manager.h"

class Control : public IInputManager::IListener {
public:
	Control(std::string name, vec2 pos, vec2 size, bool isActive) : m_name(name), m_pos(pos), m_size(size), m_isActive(isActive) {}

	virtual bool onEvent(const IInputManager::Event& event) = 0;
	virtual void update(float deltaTime) {}

	virtual void activate  ()       { m_isActive = true;  }
	virtual void deactivate()       { m_isActive = false; }
	bool         isActive  () const { return m_isActive;  }
	std::string  getName   () const { return m_name;      }

protected:
	vec2        m_pos;
	vec2        m_size;
	std::string m_name;
	bool        m_isActive;
};