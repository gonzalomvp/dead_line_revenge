#pragma once

class  Entity;
struct TMessage;

class CComponent {
public:
	CComponent(Entity* _pOwner) : m_pOwner(_pOwner), m_bIsActive(false), m_fActivationTimer(0.0f) {}
	virtual ~CComponent() = 0 {};

	virtual void init          ();
	virtual void activate      ();
	virtual void deactivate    ();
	virtual void run           (float _fDeltaTime);
	virtual void receiveMessage(TMessage* _pMessage) {}

	Entity* getOwner() { return m_pOwner; }
	void setActivationDelay(float _fActivationTimer) { m_fActivationTimer = _fActivationTimer; }

protected:
	Entity* m_pOwner;
	bool    m_bIsActive;

private:
	float   m_fActivationTimer;
};
