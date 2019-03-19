#include "common/stdafx.h"
#include "goto_random.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "entities/message.h"
#include "scenes/world.h"

void CGoToRandomPositionAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 1, "CGoToRandomPosition must have 1 param");
	mSpeed = std::stof(vParams[0]);
}

void CGoToRandomPositionAction::onEnter() {
	Entity* self = getOwnerEntity();
	MessageGetTransform messageSelfPos;
	self->receiveMessage(&messageSelfPos);

	mTargetPos = vmake(CORE_FRand(0.0f, WORLD_WIDTH), CORE_FRand(0.0f, WORLD_HEIGHT));

	mTargetPos.x = clamp(mTargetPos.x, messageSelfPos.size.x * 0.5f, WORLD_WIDTH - messageSelfPos.size.x * 0.5f);
	mTargetPos.y = clamp(mTargetPos.y, messageSelfPos.size.y * 0.5f, WORLD_HEIGHT - messageSelfPos.size.y * 0.5f);
}

Status CGoToRandomPositionAction::update(float step) {

	Entity* self = getOwnerEntity();

	MessageGetTransform messageSelfPos;
	self->receiveMessage(&messageSelfPos);

	vec2 direction = vsub(mTargetPos, messageSelfPos.pos);

	if (vlen2(direction) <= 20 * 20)
	{
		return eSuccess;
	}

	MessageSetTransform msgSetTransform;
	msgSetTransform.pos = vadd(messageSelfPos.pos, vscale(vnorm(direction), mSpeed));
	msgSetTransform.size = messageSelfPos.size;
	self->receiveMessage(&msgSetTransform);

	return eRunning;
}