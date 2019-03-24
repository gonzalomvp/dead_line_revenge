#pragma once

#include "components/AIComponent.h"

#include <vector>

class CAIFireComponent : public CAIComponent {
public:
	CAIFireComponent(Entity* _pOwner, const std::vector<vec2>& _vAimDirections = std::vector<vec2>(0), bool _bWantShuffle = false)
	: CAIComponent(_pOwner)
	, m_v2AimDirection(vmake(0.0f, 0.0f))
	, m_vAimDirections(_vAimDirections)
	, m_uCurrentAimDirectionIndex(0)
	, m_bWantShuffle(_bWantShuffle)
	{}

	// CComponent
	virtual void init()                             override;
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

private:
	vec2              m_v2AimDirection;
	std::vector<vec2> m_vAimDirections;
	size_t            m_uCurrentAimDirectionIndex;
	bool              m_bWantShuffle;
};