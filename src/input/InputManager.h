#pragma once

#include "input/IInputManager.h"

class CInputManager : public IInputManager {
public:
	~CInputManager();

	virtual void registerEvent  (IListener* _pListener, EEventType _eEventType) override;
	virtual void unregisterEvent(IListener* _pListener, EEventType _eEventType) override;
	virtual void addEvent       (CEvent* _pEvent)                               override;
	virtual void processInput   ()                                              override;

private:
	void processKeyboard();
	void checkKeyState        (int _iKey);

	void processMouse();
	void checkMouseButtonState(int _iButton, const vec2& _v2MousePos);
	
	std::vector<CEvent*>                          m_vEvents;
	std::map<int, bool>                           m_mIputStates;
	std::map<EEventType, std::vector<IListener*>> m_mlisteners;
};