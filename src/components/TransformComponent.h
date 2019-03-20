#pragma once

#include "components/component.h"

class CTransformComponent : public Component {
public:
	CTransformComponent(Entity* _pOwner, const vec2& _v2Pos, const vec2& _v2Size, const vec2& _v2SizeIncrement = vmake(0.0f, 0.0f)) :
		Component(_pOwner),
		m_v2Pos(_v2Pos),
		m_v2Size(_v2Size),
		m_v2SizeIncrement(_v2SizeIncrement) {}

	// CComponent
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

private:
	vec2 m_v2Pos;
	vec2 m_v2Size;
	vec2 m_v2SizeIncrement;
};