#include "common/stdafx.h"
#include "GoToBlackboardPosition.h"

#include "components/BehaviorTreeComponent.h"
#include "components/behavior_tree/Blackboard.h"
#include "entities/Entity.h"
#include "messages/Message.h"
#include "scenes/World.h"

#include <sstream>

void CGoToBlackboardPosition::init(TiXmlElement* _pBehaviorElem) {
	CBehavior::init(_pBehaviorElem);

	ASSERT(_pBehaviorElem);

	ASSERT(_pBehaviorElem->Attribute("sTo"));
	m_sBlackboardKey = _pBehaviorElem->Attribute("sTo");

	ASSERT(_pBehaviorElem->Attribute("fArriveDistance"));
	m_fArriveDistance = std::stof(_pBehaviorElem->Attribute("fArriveDistance"));

	m_bKeepUpdatingPosition = false;
	if (_pBehaviorElem->Attribute("bKeepUpdatingPosition")) {
		std::istringstream is(_pBehaviorElem->Attribute("bKeepUpdatingPosition"));
		is >> std::boolalpha >> m_bKeepUpdatingPosition;
	}

	if (_pBehaviorElem->Attribute("fRandomDeviation")) {
		float fRandomDeviation = std::stof(_pBehaviorElem->Attribute("fRandomDeviation"));
		m_v2Offset = vmake(CORE_FRand(-fRandomDeviation, fRandomDeviation), CORE_FRand(-fRandomDeviation, fRandomDeviation));
	}
}

void CGoToBlackboardPosition::onEnter() {
	CBehavior::onEnter();

	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity && m_pOwnerComponent);
	vec2 v2SelfSize = pOwnerEntity->getSize();

	CEntity* pTargetEntity = nullptr;
	bool bFound = m_pOwnerComponent->getBlackboard().getValueEntity(m_sBlackboardKey, pTargetEntity);
	if (bFound) {
		ASSERT(pTargetEntity);
		m_v2TargetPos = pTargetEntity->getPos();
	}
	else {
		bFound = m_pOwnerComponent->getBlackboard().getValueVec2(m_sBlackboardKey, m_v2TargetPos);
	}
	ASSERT(bFound);

	m_v2TargetPos = vadd(m_v2TargetPos, m_v2Offset);
	m_v2TargetPos.x = clamp(m_v2TargetPos.x, v2SelfSize.x * 0.5f, WORLD_WIDTH - v2SelfSize.x * 0.5f);
	m_v2TargetPos.y = clamp(m_v2TargetPos.y, v2SelfSize.y * 0.5f, WORLD_HEIGHT - v2SelfSize.y * 0.5f);
}

CBehavior::EStatus CGoToBlackboardPosition::onUpdate(float _fDeltaTime) {
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity && m_pOwnerComponent);
	vec2 v2SelfSize = pOwnerEntity->getSize();

	if (m_bKeepUpdatingPosition) {
		CEntity* pTargetEntity = nullptr;
		bool bFound = m_pOwnerComponent->getBlackboard().getValueEntity(m_sBlackboardKey, pTargetEntity);
		if (bFound) {
			ASSERT(pTargetEntity);
			m_v2TargetPos = pTargetEntity->getPos();
		}
		else {
			bFound = m_pOwnerComponent->getBlackboard().getValueVec2(m_sBlackboardKey, m_v2TargetPos);
		}
		if (!bFound) {
			return EStatus::EFail;
		}
	}

	m_v2TargetPos = vadd(m_v2TargetPos, m_v2Offset);
	m_v2TargetPos.x = clamp(m_v2TargetPos.x, v2SelfSize.x * 0.5f, WORLD_WIDTH - v2SelfSize.x * 0.5f);
	m_v2TargetPos.y = clamp(m_v2TargetPos.y, v2SelfSize.y * 0.5f, WORLD_HEIGHT - v2SelfSize.y * 0.5f);

	vec2 v2SelfToTarget = vsub(m_v2TargetPos, pOwnerEntity->getPos());

	// Stop movement if arrived
	if (vlen2(v2SelfToTarget) <= m_fArriveDistance * m_fArriveDistance) {
		TMessageSetMovementDir msgSetMovementDir;
		pOwnerEntity->receiveMessage(&msgSetMovementDir);
		return EStatus::ESuccess;
	}

	TMessageSetMovementDir msgSetMovementDir;
	msgSetMovementDir.v2Dir = v2SelfToTarget;
	pOwnerEntity->receiveMessage(&msgSetMovementDir);
	return EStatus::ERunning;
}

void CGoToBlackboardPosition::abort() {
	CBehavior::abort();

	// Stop movement
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity);
	TMessageSetMovementDir msgSetMovementDir;
	pOwnerEntity->receiveMessage(&msgSetMovementDir);
}