#pragma once

#include "components/Component.h"

class CAIComponent : public CComponent {
public:
	CAIComponent(CEntity* _pOwner) : CComponent(_pOwner) {}
	virtual ~CAIComponent() = 0 {};

	// CComponent
	virtual void receiveMessage(TMessage* _pMessage) override;
};