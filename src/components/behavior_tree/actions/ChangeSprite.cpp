#include "common/stdafx.h"
#include "ChangeSprite.h"

#include "entities/Entity.h"
#include "messages/Message.h"

void CChangeSprite::init(TiXmlElement* _pOwnerComponent) {
	CBehavior::init(_pOwnerComponent);
	ASSERT(_pOwnerComponent);

	ASSERT(_pOwnerComponent->Attribute("sSprite"));
	m_sTextureFile = _pOwnerComponent->Attribute("sSprite");
}

CBehavior::EStatus CChangeSprite::onUpdate(float step) {
	CEntity* pOwnerEntity = getOwnerEntity();
	ASSERT(pOwnerEntity);
	TMessageChangeSprite messageChangeSprite;
	messageChangeSprite.sTexture = m_sTextureFile;
	pOwnerEntity->receiveMessage(&messageChangeSprite);

	return EStatus::ESuccess;
}