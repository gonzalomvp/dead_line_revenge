#pragma once

#include "components/AIComponent.h"

#include <vector>

class CAIFireComponent : public CAIComponent {
public:
	CAIFireComponent(Entity* _pOwner, const std::vector<vec2>& _vFireDirections = std::vector<vec2>(0), bool _bWantShuffle = false)
	: CAIComponent(_pOwner)
	, m_vFireDirections(_vFireDirections)
	, m_uCurrentFireDirectionIndex(0)
	, m_bWantShuffle(_bWantShuffle)
	{}

	// CComponent
	virtual void init()                             override;
	virtual void run(float _fDeltaTime)             override;
	virtual void receiveMessage(Message* _pMessage) override;

private:
	std::vector<vec2> m_vFireDirections;
	size_t            m_uCurrentFireDirectionIndex;
	bool              m_bWantShuffle;
};