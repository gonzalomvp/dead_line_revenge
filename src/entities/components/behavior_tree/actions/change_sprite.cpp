#include "common/stdafx.h"
#include "change_sprite.h"
#include "entities/entity.h"
#include "entities/components/behavior_tree/behavior_tree.h"
#include "entities/message.h"

Status ChangeSprite::update(float step) {
	
	Entity* self = mOwner->getCharacter();
	MessageChangeSprite messageChangeSprite;
	messageChangeSprite.texture = mTexture;
	self->receiveMessage(&messageChangeSprite);

	return eSuccess;
}