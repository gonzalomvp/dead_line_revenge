#pragma once

#include "components/Component.h"

class CExplossiveComponent : public CComponent {
public:
	CExplossiveComponent(CEntity* _pOwner) : CComponent(_pOwner) {}

	// CComponent
	virtual void receiveMessage(TMessage* _pMessage) override;
};