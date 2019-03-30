#include "common/stdafx.h"
#include "GoToBlackboardPosition.h"
#include "entities/entity.h"
#include "components/BehaviorTreeComponent.h"
#include "components/behavior_tree/blackboard.h"
#include "messages/message.h"
#include "scenes/world.h"

#include <sstream>

void CGoToBlackboardPosition::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sTo"));
	m_sBlackboardKey = behaviorElem->Attribute("sTo");

	ASSERT(behaviorElem->Attribute("fArriveDistance"));
	m_fArriveDistance = std::stof(behaviorElem->Attribute("fArriveDistance"));

	m_bKeepUpdatingPosition = false;
	if (behaviorElem->Attribute("bKeepUpdatingPosition")) {
		std::istringstream is(behaviorElem->Attribute("bKeepUpdatingPosition"));
		is >> std::boolalpha >> m_bKeepUpdatingPosition;
	}

	if (behaviorElem->Attribute("fRandomDeviation")) {
		float fRandomDeviation = std::stof(behaviorElem->Attribute("fRandomDeviation"));
		m_v2Offset = vmake(CORE_FRand(-fRandomDeviation, fRandomDeviation), CORE_FRand(-fRandomDeviation, fRandomDeviation));
	}
}

void CGoToBlackboardPosition::onEnter() {
	Entity* self = getOwnerEntity();
	vec2 selfSize = self->getSize();

	Entity* pTargetEntity = nullptr;
	bool bFound = mOwner->getBlackboard().getValueEntity(m_sBlackboardKey, pTargetEntity);
	if (bFound) {
		ASSERT(pTargetEntity);
		mTargetPos = pTargetEntity->getPos();
	}
	else {
		bFound = mOwner->getBlackboard().getValueVec2(m_sBlackboardKey, mTargetPos);
	}
	ASSERT(bFound);

	mTargetPos = vadd(mTargetPos, m_v2Offset);
	mTargetPos.x = clamp(mTargetPos.x, selfSize.x * 0.5f, WORLD_WIDTH - selfSize.x * 0.5f);
	mTargetPos.y = clamp(mTargetPos.y, selfSize.y * 0.5f, WORLD_HEIGHT - selfSize.y * 0.5f);
}

EStatus CGoToBlackboardPosition::onUpdate(float step) {
	Entity* self = getOwnerEntity();
	vec2 selfSize = self->getSize();

	if (m_bKeepUpdatingPosition) {
		Entity* pTargetEntity = nullptr;
		bool bFound = mOwner->getBlackboard().getValueEntity(m_sBlackboardKey, pTargetEntity);
		if (bFound) {
			ASSERT(pTargetEntity);
			mTargetPos = pTargetEntity->getPos();
		}
		else {
			bFound = mOwner->getBlackboard().getValueVec2(m_sBlackboardKey, mTargetPos);
		}
		if (!bFound) {
			return EStatus::EFail;
		}
	}

	mTargetPos = vadd(mTargetPos, m_v2Offset);
	mTargetPos.x = clamp(mTargetPos.x, selfSize.x * 0.5f, WORLD_WIDTH - selfSize.x * 0.5f);
	mTargetPos.y = clamp(mTargetPos.y, selfSize.y * 0.5f, WORLD_HEIGHT - selfSize.y * 0.5f);

	vec2 direction = vsub(mTargetPos, self->getPos());

	if (vlen2(direction) <= m_fArriveDistance * m_fArriveDistance)
	{
		MessageSetMovementDir msgSetMovementDir;
		self->receiveMessage(&msgSetMovementDir);
		return EStatus::ESuccess;
	}

	MessageSetMovementDir msgSetMovementDir;
	msgSetMovementDir.dir = direction;
	self->receiveMessage(&msgSetMovementDir);
	return EStatus::ERunning;
}

void CGoToBlackboardPosition::abort() {
	CBehavior::abort();

	Entity* self = getOwnerEntity();
	MessageSetMovementDir msgSetMovementDir;
	self->receiveMessage(&msgSetMovementDir);
}