#pragma once

#include "components/Component.h"

class CPointsComponent : public CComponent {
public:
	CPointsComponent(CEntity* _pOwner, uint16_t _uPoints)
	: CComponent(_pOwner)
	, uPoints(_uPoints)
	{}

	// CComponent
	virtual void receiveMessage(TMessage* _pMessage) override;

private:
	uint16_t uPoints;
};
