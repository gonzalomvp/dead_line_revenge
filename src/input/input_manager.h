#pragma once

#include <map>

//=============================================================================
// IInputManager interface
//=============================================================================
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

	class Event {
	public:
		Event(TEventType type) : m_type(type) {}

		TEventType getType() const          { return m_type; };
		void       setType(TEventType type) { m_type = type; };
	private:
		TEventType m_type;
	};

	class KeyEvent : public Event {
	public:
		KeyEvent(TEventType type, int key) : Event(type), m_key(key) {}

		int        getKey() const  { return m_key; };
		void       setKey(int key) { m_key = key;  };
	private:
		int m_key;
	};

	class MouseEvent : public Event {
	public:
		MouseEvent(TEventType type, int button, const vec2& pos) : Event(type), m_button(button), m_pos(pos) {}

		vec2 getPos   () const          { return m_pos;      };
		void setPos   (const vec2& pos) { m_pos = pos;       };
		int  getButton() const          { return m_button;   };
		void setButton(int button)      { m_button = button; };
	private:
		vec2 m_pos;
		int  m_button;
	};

	class IListener {
	public:
		virtual bool onEvent(const Event& event) = 0;
	};

	virtual void registerEvent  (ptr<IListener>, TEventType eventType) = 0;
	virtual void unregisterEvent(ptr<IListener>, TEventType eventType) = 0;
	virtual void addEvent       (Event* event)                     = 0;
	virtual void processInput   ()                                 = 0;
};

//=============================================================================
// InputManager class
//=============================================================================
class InputManager : public IInputManager {
public:
	~InputManager();

	virtual void registerEvent  (ptr<IListener>, TEventType eventType);
	virtual void unregisterEvent(ptr<IListener>, TEventType eventType);
	virtual void addEvent       (ptr<Event> event);
	virtual void processInput   ();
private:
	void processKeyboard      ();
	void processMouse         ();
	void checkKeyState        (int key);
	void checkMouseButtonState(int button, const vec2& mousePos);

	std::vector<ptr<Event>>                           m_events;
	std::map<int, bool>                               m_inputStates;
	std::map<TEventType, std::vector<ptr<IListener>>> m_listenersMap;
};