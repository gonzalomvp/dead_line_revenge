#pragma once

#include "components/component.h"

class CPointsComponent : public Component {
public:
	CPointsComponent(Entity* _pOwner) : Component(_pOwner) {}

	// CComponent
	virtual void receiveMessage(Message* _pMessage) override;
};
