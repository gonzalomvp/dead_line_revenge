#pragma once

#include <map>

class IInputManager {
public:
	enum TEventType {
		EKeyDown,
		EKeyUp,
		EKeyHold,
		EMouseButtonDown,
		EMouseButtonUp,
		EMouseButtonHold,
		EMouseMove,
		EPause,
	};

	class CEvent {
	public:
		CEvent(TEventType _eEventType) : m_eType(_eEventType) {}

		TEventType getType() const                 { return m_eType;        };
		void       setType(TEventType _eEventType) { m_eType = _eEventType; };
	
	private:
		TEventType m_eType;
	};

	class CKeyEvent : public CEvent {
	public:
		CKeyEvent(TEventType _eEventType, int _iKey) : CEvent(_eEventType), m_iKey(_iKey) {}

		int        getKey() const    { return m_iKey;   };
		void       setKey(int _iKey) { m_iKey = _iKey;  };

	private:
		int m_iKey;
	};

	class CMouseEvent : public CEvent {
	public:
		CMouseEvent(TEventType _eEventType, int _iButton, const vec2& v2Pos) : CEvent(_eEventType), m_iButton(_iButton), m_v2Pos(v2Pos) {}

		vec2 getPos   () const            { return m_v2Pos;       };
		void setPos   (const vec2& v2Pos) { m_v2Pos = v2Pos;      };
		int  getButton() const            { return m_iButton;     };
		void setButton(int _iButton)      { m_iButton = _iButton; };

	private:
		vec2 m_v2Pos;
		int  m_iButton;
	};

	class IListener {
	public:
		virtual bool onEvent(const CEvent& _event) = 0;
	};

	virtual void registerEvent  (IListener* _pListener, TEventType _eEventType) = 0;
	virtual void unregisterEvent(IListener* _pListener, TEventType _eEventType) = 0;
	virtual void addEvent       (CEvent* _pEvent)                               = 0;
	virtual void processInput   ()                                              = 0;
};
