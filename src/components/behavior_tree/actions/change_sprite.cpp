#include "common/stdafx.h"
#include "change_sprite.h"
#include "entities/entity.h"
#include "components/behavior_tree/behavior_tree.h"
#include "messages/message.h"

void CChangeSpriteAction::init(TiXmlElement* behaviorElem) {
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sSprite"));
	m_sTextureFile = behaviorElem->Attribute("sSprite");
}

Status CChangeSpriteAction::update(float step) {
	
	Entity* self = getOwnerEntity();
	MessageChangeSprite messageChangeSprite;
	messageChangeSprite.texture = m_sTextureFile;
	self->receiveMessage(&messageChangeSprite);

	return eSuccess;
}