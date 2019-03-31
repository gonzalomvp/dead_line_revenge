#include "common/stdafx.h"
#include "CalculateRandomPosition.h"

#include "components/BehaviorTreeComponent.h"
#include "entities/Entity.h"
#include "messages/Message.h"
#include "scenes/World.h"

void CCalculateRandomPosition::init(TiXmlElement* _pOwnerComponent) {
	CBehavior::init(_pOwnerComponent);
	ASSERT(_pOwnerComponent);

	ASSERT(_pOwnerComponent->Attribute("sResult"));
	m_sResultBlackboardKey = _pOwnerComponent->Attribute("sResult");
}

CBehavior::EStatus CCalculateRandomPosition::onUpdate(float _fDeltaTime) {
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity && m_pOwnerComponent);
	vec2 v2SelfSize = pOwnerEntity->getSize();

	vec2 v2TargetPos = vmake(CORE_FRand(0.0f, WORLD_WIDTH), CORE_FRand(0.0f, WORLD_HEIGHT));

	v2TargetPos.x = clamp(v2TargetPos.x, v2SelfSize.x * 0.5f, WORLD_WIDTH - v2SelfSize.x * 0.5f);
	v2TargetPos.y = clamp(v2TargetPos.y, v2SelfSize.y * 0.5f, WORLD_HEIGHT - v2SelfSize.y * 0.5f);
	m_pOwnerComponent->getBlackboard().setValueVec2(m_sResultBlackboardKey, v2TargetPos);

	return EStatus::ESuccess;
}
