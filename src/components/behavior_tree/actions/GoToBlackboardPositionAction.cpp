#include "common/stdafx.h"
#include "GoToBlackboardPositionAction.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "components/behavior_tree/blackboard.h"
#include "messages/message.h"
#include "scenes/world.h"

void CGoToBlackboardPositionAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 3, "CGoToBlackboardAction must have 3 param");
	m_sBlackboardKey = vParams[0];
	m_fArriveDistance = std::stof(vParams[1]);
	m_bKeepFollowing = std::stoi(vParams[2]);

	
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
	
	if (m_bKeepFollowing && m_pTargetEntity) {
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