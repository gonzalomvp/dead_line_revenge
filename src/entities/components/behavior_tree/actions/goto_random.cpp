#include "common/stdafx.h"
#include "goto_random.h"
#include "entities/entity.h"
#include "entities/components/behavior_tree/behavior_tree.h"
#include "entities/message.h"
#include "scenes/world.h"

void CGoToRandomPosition::init(TiXmlElement* behaviorElem) {
	std::vector<std::string> params;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		params.push_back(paramElem->Attribute("value"));
	}
	mSpeed = std::stof(params[0]);
}

void CGoToRandomPosition::onEnter() {
	Entity* self = getOwnerEntity();
	MessageGetTransform messageSelfPos;
	self->receiveMessage(&messageSelfPos);

	mTargetPos = vmake(CORE_FRand(0.0f + messageSelfPos.size.x * 0.5f, WORLD_WIDTH - messageSelfPos.size.x * 0.5f), CORE_FRand(80 + messageSelfPos.size.y * 0.5f, WORLD_HEIGHT - 80 - messageSelfPos.size.y * 0.5f));
}

Status CGoToRandomPosition::update(float step) {

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