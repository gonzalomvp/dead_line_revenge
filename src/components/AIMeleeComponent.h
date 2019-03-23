#pragma once

#include "components/component.h"

class CAIMeleeComponent : public Component {
public:
	CAIMeleeComponent(Entity* _oowner, float speed, float maxDistance);

	virtual void run(float deltaTime);
	virtual void receiveMessage(Message* message);
private:
	Entity* m_pPlayer;
	float       m_speed;
	float       m_maxDistance;
	vec2        m_offset;
};