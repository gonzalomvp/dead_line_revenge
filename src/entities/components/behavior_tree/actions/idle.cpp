#include "common/stdafx.h"
#include "idle.h"
#include "entities/entity.h"
#include "entities/components/behavior_tree/behavior_tree.h"
#include "entities/message.h"

Status Idle::update(float step) {
	Entity* self = mOwner->getCharacter();
	MessageEnableAI messageEnableAI;
	messageEnableAI.enable = mEnabled;
	self->receiveMessage(&messageEnableAI);
	return eSuccess;
}