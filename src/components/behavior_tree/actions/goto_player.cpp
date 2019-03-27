#include "common/stdafx.h"
#include "goto_player.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "messages/message.h"
#include "scenes/world.h"

void CGoToPlayerPositionAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 2, "CGoToPlayerPositionAction must have 2 param");
	m_fArriveDistance = std::stof(vParams[0]);
	m_bKeepFollowing = std::stoi(vParams[1]);
}

void CGoToPlayerPositionAction::onEnter() {
	Entity* self = getOwnerEntity();
	vec2 selfSize = self->getSize();

	Entity* player = g_pWorld->getPlayer();
	vec2 playerPos = player->getPos();

	mTargetPos.x = clamp(playerPos.x, selfSize.x * 0.5f, WORLD_WIDTH - selfSize.x * 0.5f);
	mTargetPos.y = clamp(playerPos.y, selfSize.y * 0.5f, WORLD_HEIGHT - selfSize.y * 0.5f);
}

Status CGoToPlayerPositionAction::update(float step) {

	Entity* self = getOwnerEntity();
	
	if (m_bKeepFollowing) {
		vec2 selfSize = self->getSize();
		Entity* player = g_pWorld->getPlayer();
		vec2 playerPos = player->getPos();

		mTargetPos.x = clamp(playerPos.x, selfSize.x * 0.5f, WORLD_WIDTH - selfSize.x * 0.5f);
		mTargetPos.y = clamp(playerPos.y, selfSize.y * 0.5f, WORLD_HEIGHT - selfSize.y * 0.5f);
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

void CGoToPlayerPositionAction::abort() {
	CBehaviorNode::abort();

	Entity* self = getOwnerEntity();
	MessageSetMovementDir msgSetMovementDir;
	self->receiveMessage(&msgSetMovementDir);
}