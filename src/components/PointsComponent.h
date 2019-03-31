#pragma once

#include "components/Component.h"

class CPointsComponent : public CComponent {
public:
	CPointsComponent(Entity* _pOwner) : CComponent(_pOwner) {}

	// CComponent
	virtual void receiveMessage(TMessage* _pMessage) override;
};
