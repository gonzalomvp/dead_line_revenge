#include "common/stdafx.h"
#include "change_sprite.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "entities/message.h"

void CChangeSpriteAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	std::vector<std::string> vParams;
	TiXmlElement* paramElem = behaviorElem->FirstChildElement("param");
	for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
		ASSERT(paramElem->Attribute("value"), "Missing value attribute in param");
		vParams.push_back(paramElem->Attribute("value"));
	}

	ASSERT(vParams.size() == 1, "CChangeSpriteAction must have 1 param");
	m_sTextureFile = vParams[0];
}

Status CChangeSpriteAction::update(float step) {
	
	Entity* self = getOwnerEntity();
	MessageChangeSprite messageChangeSprite;
	messageChangeSprite.texture = m_sTextureFile;
	self->receiveMessage(&messageChangeSprite);

	float dist;
	mOwner->getBlackboard().getValueFloat("distance", dist);
	Entity* player;
	mOwner->getBlackboard().getValueEntity("player", player);

	return eSuccess;
}