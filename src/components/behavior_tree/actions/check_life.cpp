#include "common/stdafx.h"
#include "check_life.h"
#include "entities/entity.h"
#include "messages/message.h"
#include "scenes/world.h"
#include "components/behavior_tree/behavior_tree.h"

void CCheckLifeCondition::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("iLife"));
	m_iLife = std::stoi(behaviorElem->Attribute("iLife"));
}

Status CCheckLifeCondition::update(float step) {
	Entity* self = getOwnerEntity();
	MessageGetLife messageGetLife;
	self->receiveMessage(&messageGetLife);

	if (messageGetLife.currentLife >= m_iLife) {
		return eSuccess;
	}

	return eFail;
}