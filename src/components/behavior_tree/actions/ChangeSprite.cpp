#include "common/stdafx.h"
#include "ChangeSprite.h"
#include "entities/entity.h"
#include "components/BehaviorTreeComponent.h"
#include "messages/message.h"

void CChangeSprite::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sSprite"));
	m_sTextureFile = behaviorElem->Attribute("sSprite");
}

EStatus CChangeSprite::onUpdate(float step) {
	
	Entity* self = getOwnerEntity();
	MessageChangeSprite messageChangeSprite;
	messageChangeSprite.texture = m_sTextureFile;
	self->receiveMessage(&messageChangeSprite);

	return EStatus::ESuccess;
}