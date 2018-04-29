#pragma once
#include <vector>
#include <map>

#define MOUSE_LBUTTON 0x0001
#define MOUSE_RBUTTON 0x0010
#define MOUSE_MBUTTON 0x0100

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
			KeyDown,
		};
		int key;
		KeyAction action;
	};

	class IListener {
	public:
		virtual bool onEvent(const Event&) = 0;
	};

	virtual int registerEvent(IListener*, TEvent e, int priority) = 0;
	virtual int unregisterEvent(IListener*, TEvent event) = 0;
	virtual void addEvent(Event* event) = 0;
	virtual void processInput() = 0;
};

class InputManager : public IInputManager {
public:
	InputManager();
	~InputManager();
	virtual int registerEvent(IListener*, TEvent e, int priority);
	virtual int unregisterEvent(IListener*, TEvent event);
	virtual void processInput();
	virtual void addEvent(Event* event);
	//void clearListeners() { m_listeners.clear(); }

private:
	void proceesKeyboard();
	void proceesMouse();
	void checkKeyState(int key);
	//std::vector<IListener*> m_listeners;
	std::vector<Event*> m_events;
	std::map<int, bool> m_keys;
	bool m_lButtonPressed;
	bool m_rButtonPressed;
	bool m_mButtonPressed;
	std::map<TEvent, std::vector<IListener*>> m_listenersMap;
};