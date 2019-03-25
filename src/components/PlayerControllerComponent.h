#pragma once

#include "components/component.h"
#include "input/input_manager.h"

class CPlayerControllerComponent : public Component, public IInputManager::IListener {
public:
	CPlayerControllerComponent(Entity* _pOwner)
	: Component(_pOwner)
	, m_v2InputDir(vmake(0.0f, 0.0f))
	{}

	~CPlayerControllerComponent();

	// CComponent
	virtual void init()                             override;
	virtual void run(float _fDeltaTime)             override;

	// IInputManager::IListener
	virtual bool onEvent(const IInputManager::CEvent& _event) override;

private:
	vec2 m_v2InputDir;
};