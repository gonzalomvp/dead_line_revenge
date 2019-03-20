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

	ASSERT(vParams.size() == 1, "CGoToPlayerPositionAction must have 1 param");
	m_fSpeed = std::stof(vParams[0]);
}

void CGoToPlayerPositionAction::onEnter() {
	Entity* self = getOwnerEntity();
	MessageGetTransform messageSelfPos;
	self->receiveMessage(&messageSelfPos);

	Entity* player = g_pWorld->getPlayer();
	MessageGetTransform messagePlayerPos;
	player->receiveMessage(&messagePlayerPos);

	mTargetPos.x = clamp(messagePlayerPos.pos.x, messageSelfPos.size.x * 0.5f, WORLD_WIDTH - messageSelfPos.size.x * 0.5f);
	mTargetPos.y = clamp(messagePlayerPos.pos.y, messageSelfPos.size.y * 0.5f, WORLD_HEIGHT - messageSelfPos.size.y * 0.5f);
}

Status CGoToPlayerPositionAction::update(float step) {

	Entity* self = getOwnerEntity();

	MessageGetTransform messageSelfPos;
	self->receiveMessage(&messageSelfPos);

	vec2 direction = vsub(mTargetPos, messageSelfPos.pos);

	if (vlen2(direction) <= 20 * 20)
	{
		return eSuccess;
	}

	MessageSetTransform msgSetTransform;
	msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(direction), m_fSpeed));
	msgSetTransform.size = messageSelfPos.size;
	self->receiveMessage(&msgSetTransform);

	return eRunning;
}