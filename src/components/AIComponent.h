#pragma once

#include "components/component.h"

class CAIComponent : public Component {
public:
	CAIComponent(Entity* _pOwner) : Component(_pOwner) {}

	// CComponent
	virtual void receiveMessage(Message* _pMessage) override;
};