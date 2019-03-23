#pragma once

class  Entity;
struct Message;

class Component {
public:
	Component(Entity* owner) : m_owner(owner), m_isActive(false), m_activationDelay(0), m_activationTimer(0) {}
	virtual ~Component() = 0 {};

	virtual void init          ();
	virtual void activate      ();
	virtual void deactivate    ();
	virtual void run           (float deltaTime);
	virtual void receiveMessage(Message* message) {}

	void setActivationDelay(int activationDelay) { m_activationDelay = activationDelay; }
	Entity* getOwner() { return m_owner; }

protected:
	Entity* m_owner;
	bool        m_isActive;
	int         m_activationDelay;
	int         m_activationTimer;
};
