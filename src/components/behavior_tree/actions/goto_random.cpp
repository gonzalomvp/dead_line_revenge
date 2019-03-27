#include "common/stdafx.h"
#include "goto_random.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "messages/message.h"
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
	m_fArriveDistance = std::stof(vParams[0]);
}

void CGoToRandomPositionAction::onEnter() {
	Entity* self = getOwnerEntity();
	vec2 selfSize = self->getSize();

	mTargetPos = vmake(CORE_FRand(0.0f, WORLD_WIDTH), CORE_FRand(0.0f, WORLD_HEIGHT));

	mTargetPos.x = clamp(mTargetPos.x, selfSize.x * 0.5f, WORLD_WIDTH - selfSize.x * 0.5f);
	mTargetPos.y = clamp(mTargetPos.y, selfSize.y * 0.5f, WORLD_HEIGHT - selfSize.y * 0.5f);


	vec2 playerPos = g_pWorld->getPlayer()->getPos();

	vec2 playerToEnemy = vnorm(vsub(self->getPos(),playerPos));
	mTargetPos = vadd(self->getPos(), vscale(playerToEnemy, 300.0f));

	float fAngleStep = 10.f * (rand() % 2 - 0.5f) * 2.0f;
	int iStep = 0;
	float fInitAngle = vangle(playerToEnemy);
	while (mTargetPos.x < selfSize.x * 0.5f || mTargetPos.x > WORLD_WIDTH - selfSize.x * 0.5f 
		|| mTargetPos.y < selfSize.y * 0.5f || mTargetPos.y > WORLD_HEIGHT - selfSize.y * 0.5f) {
		int iNumStep = iStep / 2 + 1;
		float fSign = (iStep % 2 - 0.5f) * 2.0f;

		float fAngle = fInitAngle + fAngleStep * iNumStep * fSign;
		mTargetPos = vadd(self->getPos(), vscale(vunit(DEG2RAD(fAngle)), 300.0f));
		++iStep;
	}
}

Status CGoToRandomPositionAction::update(float step) {
	Entity* self = getOwnerEntity();
	vec2 direction = vsub(mTargetPos, self->getPos());

	if (vlen2(direction) <= m_fArriveDistance * m_fArriveDistance) {
		MessageSetMovementDir msgSetMovementDir;
		self->receiveMessage(&msgSetMovementDir);
		return eSuccess;
	}

	MessageSetMovementDir msgSetMovementDir;
	msgSetMovementDir.dir = direction;
	self->receiveMessage(&msgSetMovementDir);

	return eRunning;
}

void CGoToRandomPositionAction::abort() {
	CBehaviorNode::abort();

	Entity* self = getOwnerEntity();
	MessageSetMovementDir msgSetMovementDir;
	self->receiveMessage(&msgSetMovementDir);
}