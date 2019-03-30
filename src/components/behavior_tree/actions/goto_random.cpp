#include "common/stdafx.h"
#include "goto_random.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "messages/message.h"
#include "scenes/world.h"

void CCalculateRandomPositionAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sResult"));
	m_sResultBlackboardKey = behaviorElem->Attribute("sResult");
}

Status CCalculateRandomPositionAction::update(float step) {
	Entity* self = getOwnerEntity();
	vec2 selfSize = self->getSize();

	vec2 v2TargetPos = vmake(CORE_FRand(0.0f, WORLD_WIDTH), CORE_FRand(0.0f, WORLD_HEIGHT));

	v2TargetPos.x = clamp(v2TargetPos.x, selfSize.x * 0.5f, WORLD_WIDTH - selfSize.x * 0.5f);
	v2TargetPos.y = clamp(v2TargetPos.y, selfSize.y * 0.5f, WORLD_HEIGHT - selfSize.y * 0.5f);

	mOwner->getBlackboard().setValueVec2(m_sResultBlackboardKey, v2TargetPos);
	return eSuccess;
}
