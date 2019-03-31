#include "common/stdafx.h"
#include "AIComponent.h"

#include "messages/Message.h"

void CAIComponent::receiveMessage(TMessage* _pMessage) {
	CComponent::receiveMessage(_pMessage);

	ASSERT(_pMessage);

	if (TMessageEnableAI* pMessage = dynamic_cast<TMessageEnableAI*>(_pMessage)) {
		if (pMessage->enable) {
			activate();
		}
		else {
			deactivate();
		}
	}
}