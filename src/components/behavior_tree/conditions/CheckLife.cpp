#include "common/stdafx.h"
#include "CheckLife.h"
#include "entities/Entity.h"
#include "messages/Message.h"
#include "scenes/World.h"
#include "components/BehaviorTreeComponent.h"

void CCheckLife::init(TiXmlElement* behaviorElem) {
	CCondition::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("iLife"));
	m_iLife = std::stoi(behaviorElem->Attribute("iLife"));
}

bool CCheckLife::check(float step) {
	Entity* self = getOwnerEntity();
	TMessageGetLife messageGetLife;
	self->receiveMessage(&messageGetLife);

	return messageGetLife.iCurrentLife >= m_iLife;
}