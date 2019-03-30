#include "common/stdafx.h"
#include "CalculateRandomPosition.h"
#include "entities/entity.h"
#include "components/BehaviorTreeComponent.h"
#include "messages/message.h"
#include "scenes/world.h"

void CCalculateRandomPosition::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sResult"));
	m_sResultBlackboardKey = behaviorElem->Attribute("sResult");
}

EStatus CCalculateRandomPosition::onUpdate(float step) {
	Entity* self = getOwnerEntity();
	vec2 selfSize = self->getSize();

	vec2 v2TargetPos = vmake(CORE_FRand(0.0f, WORLD_WIDTH), CORE_FRand(0.0f, WORLD_HEIGHT));

	v2TargetPos.x = clamp(v2TargetPos.x, selfSize.x * 0.5f, WORLD_WIDTH - selfSize.x * 0.5f);
	v2TargetPos.y = clamp(v2TargetPos.y, selfSize.y * 0.5f, WORLD_HEIGHT - selfSize.y * 0.5f);

	mOwner->getBlackboard().setValueVec2(m_sResultBlackboardKey, v2TargetPos);
	return EStatus::ESuccess;
}
