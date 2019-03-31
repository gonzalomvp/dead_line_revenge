#include "common/stdafx.h"
#include "CalculateFleePosition.h"

#include "components/BehaviorTreeComponent.h"
#include "entities/Entity.h"
#include "messages/Message.h"
#include "scenes/World.h"

namespace {
	const float s_fAngleStep = 10.f;
}

void CCalculateFleePosition::init(TiXmlElement* _pBehaviorElem) {
	CBehavior::init(_pBehaviorElem);
	ASSERT(_pBehaviorElem);

	ASSERT(_pBehaviorElem->Attribute("sFrom"));
	m_sFromBlackboardKey = _pBehaviorElem->Attribute("sFrom");

	ASSERT(_pBehaviorElem->Attribute("sResult"));
	m_sResultBlackboardKey = _pBehaviorElem->Attribute("sResult");

	ASSERT(_pBehaviorElem->Attribute("fDistance"));
	m_fDistance = std::stof(_pBehaviorElem->Attribute("fDistance"));
}

CBehavior::EStatus CCalculateFleePosition::onUpdate(float step) {
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity && m_pOwnerComponent);
	vec2 v2SelfSize = pOwnerEntity->getSize();

	CEntity* pFromEntity = nullptr;
	vec2 v2FromPos = vmake(0.0f, 0.0f);

	bool bFound = m_pOwnerComponent->getBlackboard().getValueEntity(m_sFromBlackboardKey, pFromEntity);
	if (bFound) {
		ASSERT(pFromEntity);
		v2FromPos = pFromEntity->getPos();
	}
	else {
		bFound = m_pOwnerComponent->getBlackboard().getValueVec2(m_sFromBlackboardKey, v2FromPos);
	}
	if (!bFound) {
		return EStatus::EFail;
	}

	vec2 v2ToEnemyDir = vnorm(vsub(pOwnerEntity->getPos(), v2FromPos));
	vec2 v2TargetPos = vadd(v2FromPos, vscale(v2ToEnemyDir, m_fDistance));

	float fAngleStep = DEG2RAD(s_fAngleStep) * (rand() % 2 - 0.5f) * 2.0f;
	int iStep = 0;
	float fInitAngle = vangle(v2ToEnemyDir);

	// find a valid position in the world changing progressively the angle
	while (v2TargetPos.x < v2SelfSize.x * 0.5f || v2TargetPos.x > WORLD_WIDTH - v2SelfSize.x * 0.5f
		|| v2TargetPos.y < v2SelfSize.y * 0.5f || v2TargetPos.y > WORLD_HEIGHT - v2SelfSize.y * 0.5f) {
		int iNumStep = iStep / 2 + 1;
		float fSign = (iStep % 2 - 0.5f) * 2.0f;
		float fAngle = fInitAngle + (fAngleStep * iNumStep * fSign);
		v2TargetPos = vadd(v2FromPos, vscale(vunit(fAngle), m_fDistance));
		++iStep;

		// completed a 360 search but not position found
		if (iStep >= (360.0f / s_fAngleStep)) {
			return EStatus::EFail;
		}
	}
	m_pOwnerComponent->getBlackboard().setValueVec2(m_sResultBlackboardKey, v2TargetPos);

	return EStatus::ESuccess;
}
