#pragma once

#include "entities/components/component.h"

#include "rapidjson/document.h"

class Entity;

class CMovementComponent : public Component {
public:
	static Component* loadComponent(Entity* _pOwner, const rapidjson::Value* _pComponentInfo);

	CMovementComponent(Entity* _pOwner, const vec2& _vDirection, float _fSpeed, bool _bHasInertia, bool _bHasBounce) :
		Component(_pOwner),
		m_vDirection(_vDirection),
		m_fSpeed(_fSpeed),
		m_bHasInertia(_bHasInertia),
		m_bHasBounce(_bHasBounce) {}

	virtual void run(float _fDeltaTime);
	virtual void receiveMessage(Message* _pMessage);
private:
	vec2  m_vDirection;
	float m_fSpeed;
	bool  m_bHasInertia;
	bool  m_bHasBounce;
};