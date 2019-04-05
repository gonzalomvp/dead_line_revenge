#pragma once

#include "input/IInputManager.h"

class CControl : public IInputManager::IListener {
public:
	CControl(const std::string& _sName, const vec2& _v2Pos, const vec2& _v2Size, bool _bIsActive = true) : m_sName(_sName), m_v2Pos(_v2Pos), m_v2Size(_v2Size), m_bIsActive(_bIsActive) {}
	virtual ~CControl() = 0 {};

	virtual void init      ()                  {                      }
	virtual void run       (float _fDeltaTime) {                      }
	virtual void activate  ()                  { m_bIsActive = true;  }
	virtual void deactivate()                  { m_bIsActive = false; }

	std::string getName () const { return m_sName;     }
	vec2        getPos  () const { return m_v2Pos;     }
	vec2        getSize () const { return m_v2Size;    }
	bool        isActive() const { return m_bIsActive; }

	// IInputManager::IListener
	virtual bool onEvent(const IInputManager::CEvent& _event) { return false; }

protected:
	std::string m_sName;
	vec2        m_v2Pos;
	vec2        m_v2Size;
	bool        m_bIsActive;
};