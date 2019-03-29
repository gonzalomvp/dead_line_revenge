#include "common/stdafx.h"
#include "CalculateFleePositionAction.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "messages/message.h"
#include "scenes/world.h"

namespace {
	const float s_fAngleStep = 10.f;
}

void CCalculateFleePositionAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sFrom"));
	m_sFromBlackboardKey = behaviorElem->Attribute("sFrom");

	ASSERT(behaviorElem->Attribute("sResult"));
	m_sResultBlackboardKey = behaviorElem->Attribute("sResult");

	ASSERT(behaviorElem->Attribute("fDistance"));
	m_fDistance = std::stof(behaviorElem->Attribute("fDistance"));
}

Status CCalculateFleePositionAction::update(float step) {
	ASSERT(mOwner);

	Entity* self = getOwnerEntity();
	vec2 selfSize = self->getSize();

	Entity* pFromEntity;
	vec2 v2FromPos = vmake(0.0f, 0.0f);

	bool bFound = mOwner->getBlackboard().getValueEntity(m_sFromBlackboardKey, pFromEntity);
	// The blackboard key is an Entity
	if (bFound) {
		ASSERT(pFromEntity);
		v2FromPos = pFromEntity->getPos();
	}
	// The blackboard key is vec2
	else {
		bFound = mOwner->getBlackboard().getValueVec2(m_sFromBlackboardKey, v2FromPos);
	}
	if (!bFound) {
		return eFail;
	}

	vec2 v2ToEnemyDir = vnorm(vsub(self->getPos(), v2FromPos));
	vec2 v2TargetPos = vadd(self->getPos(), vscale(v2ToEnemyDir, m_fDistance));

	float fAngleStep = s_fAngleStep * (rand() % 2 - 0.5f) * 2.0f;
	int iStep = 0;
	float fInitAngle = vangle(v2ToEnemyDir);

	// find a valid pos in the world changing progressively the angle
	while (v2TargetPos.x < selfSize.x * 0.5f || v2TargetPos.x > WORLD_WIDTH - selfSize.x * 0.5f
		|| v2TargetPos.y < selfSize.y * 0.5f || v2TargetPos.y > WORLD_HEIGHT - v2TargetPos.y * 0.5f) {
		int iNumStep = iStep / 2 + 1;
		float fSign = (iStep % 2 - 0.5f) * 2.0f;
		float fAngle = fInitAngle + fAngleStep * iNumStep * fSign;
		v2TargetPos = vadd(self->getPos(), vscale(vunit(DEG2RAD(fAngle)), m_fDistance));
		++iStep;

		if (iStep >= (360.0 / s_fAngleStep)) {
			return eFail;
		}
	}

	mOwner->getBlackboard().setValueVec2(m_sResultBlackboardKey, v2TargetPos);

	return eSuccess;
}
