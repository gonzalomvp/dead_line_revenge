#pragma once
#include <vector>
#include <map>

class MouseInputManager;

class IInputManager {
public:
	enum TEvent {
		EMouse,
		EKey,
		EQuit,
		EPause,
	};

	class Event {
	public:
		TEvent getType() const { return m_type; };
		void   setType(TEvent type) { m_type = type; };
	private:
		TEvent m_type;
	};

	class MouseEvent : public Event {
	public:
		enum MouseButton {
			BNone,
			BLeft,
			BRight,
			BMiddle,
		};

		enum MouseAction {
			AMove,
			AButtonDown,
			AButtonUp,
		};

		int x;
		int y;
		int buttonMask;
		MouseButton mouseButton;
		MouseAction mouseButtonAction;
	};

	class KeyEvent : public Event {
	public:
		enum KeyAction {
			KeyPressed,
			KeyReleased,
		};
		int key;
		KeyAction action;
	};

	class IListener {
	public:
		virtual bool onEvent(const Event&) = 0;
	};

	virtual int registerEvent(IListener*, TEvent e, int priority) = 0;
	virtual int unregisterEvent(IListener*) = 0;
	virtual void processInput() = 0;
};

class InputManager : public IInputManager {
public:
	InputManager();
	~InputManager();
	virtual int registerEvent(IListener*, TEvent e, int priority);
	virtual int unregisterEvent(IListener*);
	virtual void processInput();
	void addEvent(Event* event);
	void clearListeners() { m_listeners.clear(); }

private:
	void checkKeyState(int key);
	std::vector<IListener*> m_listeners;
	std::vector<Event*> m_events;
	MouseInputManager* m_mouseInputManager;
	std::map<int, bool> m_keys;
};