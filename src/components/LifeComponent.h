#pragma once

#include "components/component.h"

#include "rapidjson/document.h"

class Entity;

class CLifeComponent : public Component {
public:
	CLifeComponent(Entity* _pOwner, int _iLife, int _iTimeToLive, int _iInvencibleTime) :
		Component(_pOwner),
		m_iLife(_iLife),
		m_iTimeToLive(_iTimeToLive),
		m_iInvencibleTime(_iInvencibleTime),
		m_iLifeTimer(0),
		m_iInvencibleTimer(0) {}

	virtual void run(float _fDeltaTime);
	virtual void receiveMessage(Message* _pMessage);

	static Component* loadComponent(Entity* _pOwner, const rapidjson::Value* _pComponentInfo);
private:
	int m_iLife;
	int m_iTimeToLive;
	int m_iInvencibleTime;

	//Timers
	int m_iLifeTimer;
	int m_iInvencibleTimer;
};