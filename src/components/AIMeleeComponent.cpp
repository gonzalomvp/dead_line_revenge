#include "common/stdafx.h"
#include "AIMeleeComponent.h"

#include "messages/message.h"
#include "scenes/world.h"

CAIMeleeComponent::CAIMeleeComponent(Entity* _pOwner, float _fSpeed, float _fMaxDistance) : Component(_pOwner), m_fSpeed(_fSpeed), m_fMaxDistance(_fMaxDistance) {
	m_v2Offset = vmake(CORE_FRand(-20.0f, 20.0f), CORE_FRand(-20.0f, 20.0f));
}

void CAIMeleeComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_isActive)
		return;

	ASSERT(g_pWorld && g_pWorld->getPlayer());

	MessageGetTransform messageSelfPos;
	m_owner->receiveMessage(&messageSelfPos);
	MessageGetTransform messagePlayerPos;
	g_pWorld->getPlayer()->receiveMessage(&messagePlayerPos);
	vec2 v2EnemyToPlayer = vsub(vadd(messagePlayerPos.pos, m_v2Offset), messageSelfPos.pos);

	if (vlen2(v2EnemyToPlayer) > m_fMaxDistance * m_fMaxDistance) {
		MessageSetTransform msgSetTransform;
		msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(v2EnemyToPlayer), m_fSpeed));
		msgSetTransform.size = messageSelfPos.size;
		m_owner->receiveMessage(&msgSetTransform);
	}
}

void CAIMeleeComponent::receiveMessage(Message* _pMessage) {
	ASSERT(_pMessage);

	if (MessageEnableAI* pMessage = dynamic_cast<MessageEnableAI*>(_pMessage)) {
		if (pMessage->enable) {
			activate();
		}
		else {
			deactivate();
		}
	}
}