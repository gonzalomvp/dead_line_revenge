#pragma once

#include "components/component.h"
#include "input/input_manager.h"

class CPlayerControllerComponent : public Component, public IInputManager::IListener {
public:
	CPlayerControllerComponent(Entity* _pOwner) : Component(_pOwner) {}
	~CPlayerControllerComponent();

	// CComponent
	virtual void init() override;

	// IInputManager::IListener
	virtual bool onEvent(const IInputManager::CEvent& _event) override;
};