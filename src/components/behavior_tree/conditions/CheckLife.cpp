#include "common/stdafx.h"
#include "CheckLife.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/BehaviorTreeComponent.h"

void CCheckLife::init(TiXmlElement* behaviorElem) {
	CCondition::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("iLife"));
	m_iLife = std::stoi(behaviorElem->Attribute("iLife"));
}

bool CCheckLife::check(float step) {
	Entity* self = getOwnerEntity();
	MessageGetLife messageGetLife;
	self->receiveMessage(&messageGetLife);

	return messageGetLife.currentLife >= m_iLife;
}