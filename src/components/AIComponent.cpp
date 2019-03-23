#include "common/stdafx.h"
#include "AIComponent.h"

#include "messages/message.h"

void CAIComponent::receiveMessage(Message* _pMessage) {
	Component::receiveMessage(_pMessage);

	ASSERT(_pMessage);

	if (MessageEnableAI* pMessage = dynamic_cast<MessageEnableAI*>(_pMessage)) {
		if (pMessage->enable) {
			activate();
		}
		else {
			deactivate();
		}
	}
}