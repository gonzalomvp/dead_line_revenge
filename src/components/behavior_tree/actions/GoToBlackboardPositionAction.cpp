#include "common/stdafx.h"
#include "GoToBlackboardPositionAction.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "components/behavior_tree/blackboard.h"
#include "messages/message.h"
#include "scenes/world.h"

#include <sstream>

void CGoToBlackboardPositionAction::init(TiXmlElement* behaviorElem) {
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
}

void CGoToBlackboardPositionAction::onEnter() {
	Entity* self = getOwnerEntity();
	vec2 selfSize = self->getSize();

	bool bFound = mOwner->getBlackboard().getValueEntity(m_sBlackboardKey, m_pTargetEntity);
	if (bFound) {
		ASSERT(m_pTargetEntity);
		mTargetPos = m_pTargetEntity->getPos();
	}
	else {
		bFound = mOwner->getBlackboard().getValueVec2(m_sBlackboardKey, mTargetPos);
	}
	ASSERT(bFound);

	mTargetPos.x = clamp(mTargetPos.x, selfSize.x * 0.5f, WORLD_WIDTH - selfSize.x * 0.5f);
	mTargetPos.y = clamp(mTargetPos.y, selfSize.y * 0.5f, WORLD_HEIGHT - selfSize.y * 0.5f);
}

Status CGoToBlackboardPositionAction::update(float step) {

	Entity* self = getOwnerEntity();
	
	//volver a coger del blackboard, puede haber muerto
	if (m_bKeepUpdatingPosition && m_pTargetEntity) {
		vec2 selfSize = self->getSize();
		mTargetPos = m_pTargetEntity->getPos();

		mTargetPos.x = clamp(mTargetPos.x, selfSize.x * 0.5f, WORLD_WIDTH - selfSize.x * 0.5f);
		mTargetPos.y = clamp(mTargetPos.y, selfSize.y * 0.5f, WORLD_HEIGHT - selfSize.y * 0.5f);
	}
	

	vec2 direction = vsub(mTargetPos, self->getPos());

	if (vlen2(direction) <= m_fArriveDistance * m_fArriveDistance)
	{
		MessageSetMovementDir msgSetMovementDir;
		self->receiveMessage(&msgSetMovementDir);
		return eSuccess;
	}

	MessageSetMovementDir msgSetMovementDir;
	msgSetMovementDir.dir = direction;
	self->receiveMessage(&msgSetMovementDir);

	return eRunning;
}

void CGoToBlackboardPositionAction::abort() {
	CBehaviorNode::abort();

	Entity* self = getOwnerEntity();
	MessageSetMovementDir msgSetMovementDir;
	self->receiveMessage(&msgSetMovementDir);
}