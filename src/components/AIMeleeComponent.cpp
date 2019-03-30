#include "common/stdafx.h"
#include "AIMeleeComponent.h"

#include "messages/message.h"
#include "scenes/world.h"

CAIMeleeComponent::CAIMeleeComponent(Entity* _pOwner, float _fSpeed, float _fMaxDistance)
: CAIComponent(_pOwner)
, m_fSpeed(_fSpeed)
, m_fMaxDistance(_fMaxDistance) {
	m_v2Offset = vmake(CORE_FRand(-20.0f, 20.0f), CORE_FRand(-20.0f, 20.0f));
}

void CAIMeleeComponent::run(float _fDeltaTime) {
	Component::run(_fDeltaTime);
	if (!m_bIsActive)
		return;

	ASSERT(m_pOwner && g_pWorld && g_pWorld->getPlayer());

	//MessageGetTransform messageSelfPos;
	//m_pOwner->receiveMessage(&messageSelfPos);
	//MessageGetTransform messagePlayerPos;
	//g_pWorld->getPlayer()->receiveMessage(&messagePlayerPos);
	//vec2 v2EnemyToPlayer = vsub(vadd(messagePlayerPos.pos, m_v2Offset), messageSelfPos.pos);

	//if (vlen2(v2EnemyToPlayer) > m_fMaxDistance * m_fMaxDistance) {
	//	MessageSetTransform msgSetTransform;
	//	msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(v2EnemyToPlayer), m_fSpeed));
	//	msgSetTransform.size = messageSelfPos.size;
	//	m_pOwner->receiveMessage(&msgSetTransform);
	//}
}
