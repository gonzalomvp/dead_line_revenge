#pragma once

#include "components/Component.h"

class CExplossiveComponent : public CComponent {
public:
	CExplossiveComponent(Entity* _pOwner) : CComponent(_pOwner) {}

	// CComponent
	virtual void receiveMessage(Message* _pMessage) override;
};