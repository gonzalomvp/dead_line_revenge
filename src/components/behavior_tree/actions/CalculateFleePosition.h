#pragma once

#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CCalculateFleePosition : public CBehavior {
public:
	CCalculateFleePosition(CBehaviorTreeComponent* _pOwnerComponent)
	: CBehavior(_pOwnerComponent)
	, m_sFromBlackboardKey("")
	, m_sResultBlackboardKey("")
	, m_fDistance(0.0f)
	{}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	// CBehavior
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	std::string m_sFromBlackboardKey;
	std::string m_sResultBlackboardKey;
	float       m_fDistance;
};

