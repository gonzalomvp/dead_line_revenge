#pragma once

#include "components/Component.h"

class CExplossiveComponent : public Component {
public:
	CExplossiveComponent(Entity* _pOwner) : Component(_pOwner) {}

	// CComponent
	virtual void receiveMessage(Message* _pMessage) override;
};