#pragma once

#include "../common/core.h"
#include "../common/font.h"
#include "../common/stdafx.h"
#include "../common/sys.h"


#include "../input/input_manager.h"

class Sprite;
class MenuItem;

//Preguntar a Tomas
class GUIManager {
public:
	static GUIManager* instance() { if (!mInstance) mInstance = new GUIManager(); return mInstance; }
	//void update();
	void render();
	//virtual bool onEvent(const IInputManager::Event&);
private:
	static GUIManager* mInstance;
	GUIManager();
	~GUIManager() {};
	std::vector<MenuItem*> m_menuItems;
};

class Control : public IInputManager::IListener {
public:
	Control(std::string name, vec2 pos, vec2 size /*, Container* parent*/) : m_name(name), m_pos(pos), m_size(size), m_isActive(true) {}
	~Control();
	virtual bool onEvent(const IInputManager::Event&) { return false; }
	virtual void update(float deltaTime) {}
	virtual void activate() { m_isActive = true; }
	virtual void deactivate() { m_isActive = false; }

	std::string getName() { return m_name; }

protected:
	vec2 m_pos;
	vec2 m_size;
	std::string m_name;
	bool m_isActive;
};

//class Button : public Control {
//public:
//	class IListener {
//	public:
//		virtual void onClick(Button*) = 0;
//	};
//
//	Button(std::string name, vec2 pos, vec2 size /*, Container* parent*/, const char* spriteOn, const char* spriteOff);
//	virtual bool onEvent(const IInputManager::Event&);
//	virtual void update(float deltaTime) {}
//	void addListener(IListener* listener) { m_listeners.push_back(listener); }
//
//private:
//	Sprite* m_spriteOn;
//	Sprite* m_spriteOff;
//	std::vector<IListener*> m_listeners;
//	bool m_pressed;
//};
