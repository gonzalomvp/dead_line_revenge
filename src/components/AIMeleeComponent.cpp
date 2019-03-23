#include "common/stdafx.h"
#include "AIMeleeComponent.h"

#include "messages/message.h"
#include "scenes/world.h"

CAIMeleeComponent::CAIMeleeComponent(Entity* owner, float speed, float maxDistance) : Component(owner), m_speed(speed), m_maxDistance(maxDistance) {
	m_offset = vmake(CORE_FRand(-20, 20), CORE_FRand(-20, 20));
}
void CAIMeleeComponent::run(float deltaTime) {
	Component::run(deltaTime);
	if (!m_isActive)
		return;

	MessageGetTransform messageSelfPos;
	m_owner->receiveMessage(&messageSelfPos);
	MessageGetTransform messagePlayerPos;
	m_pPlayer->receiveMessage(&messagePlayerPos);
	vec2 direction = vsub(vadd(messagePlayerPos.pos, m_offset), messageSelfPos.pos);

	if (vlen(direction) > m_maxDistance) {
		MessageSetTransform msgSetTransform;
		msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(direction), m_speed));
		msgSetTransform.size = messageSelfPos.size;
		m_owner->receiveMessage(&msgSetTransform);
	}
}

void CAIMeleeComponent::receiveMessage(Message* message) {
	MessageEnableAI* msgEnableAI = dynamic_cast<MessageEnableAI*>(message);
	if (msgEnableAI) {
		if (msgEnableAI->enable) {
			activate();
		}
		else {
			deactivate();
		}
	}
}