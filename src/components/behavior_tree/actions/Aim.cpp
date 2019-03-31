#include "common/stdafx.h"
#include "Aim.h"

#include "components/BehaviorTreeComponent.h"
#include "entities/Entity.h"
#include "messages/Message.h"

void CAim::init(TiXmlElement* _pOwnerComponent) {
	CBehavior::init(_pOwnerComponent);
	ASSERT(_pOwnerComponent);

	ASSERT(_pOwnerComponent->Attribute("sTo"));
	m_sBlackboardKey = _pOwnerComponent->Attribute("sTo");
}

CBehavior::EStatus CAim::onUpdate(float _fDeltaTime) {
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity && m_pOwnerComponent);

	CEntity* pTargetEntity = nullptr;
	vec2 v2TargetPos = vmake(0.0f, 0.0f);

	bool bFound = m_pOwnerComponent->getBlackboard().getValueEntity(m_sBlackboardKey, pTargetEntity);
	if (bFound) {
		ASSERT(pTargetEntity);
		v2TargetPos = pTargetEntity->getPos();
	}
	else {
		bFound = m_pOwnerComponent->getBlackboard().getValueVec2(m_sBlackboardKey, v2TargetPos);
	}
	if (!bFound) {
		return EStatus::EFail;
	}

	TMessageSetAimDirection messageSetAimDirection;
	messageSetAimDirection.v2Dir = vnorm(vsub(v2TargetPos, pOwnerEntity->getPos()));
	pOwnerEntity->receiveMessage(&messageSetAimDirection);

	return EStatus::ESuccess;
}