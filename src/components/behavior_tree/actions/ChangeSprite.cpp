#include "common/stdafx.h"
#include "ChangeSprite.h"
#include "entities/Entity.h"
#include "components/BehaviorTreeComponent.h"
#include "messages/Message.h"

void CChangeSprite::init(TiXmlElement* behaviorElem) {
	CBehavior::init(behaviorElem);
	ASSERT(behaviorElem);

	ASSERT(behaviorElem->Attribute("sSprite"));
	m_sTextureFile = behaviorElem->Attribute("sSprite");
}

EStatus CChangeSprite::onUpdate(float step) {
	
	Entity* self = getOwnerEntity();
	TMessageChangeSprite messageChangeSprite;
	messageChangeSprite.sTexture = m_sTextureFile;
	self->receiveMessage(&messageChangeSprite);

	return EStatus::ESuccess;
}