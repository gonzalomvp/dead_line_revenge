#pragma once

#include "components/Component.h"

class CAIComponent : public CComponent {
public:
	CAIComponent(Entity* _pOwner) : CComponent(_pOwner) {}
	virtual ~CAIComponent() = 0 {};

	// CComponent
	virtual void receiveMessage(Message* _pMessage) override;
};