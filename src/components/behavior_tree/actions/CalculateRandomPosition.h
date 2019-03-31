#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CCalculateRandomPosition : public CBehavior {
public:
	CCalculateRandomPosition(CBehaviorTreeComponent* _pOwnerComponent)
	: CBehavior(_pOwnerComponent)
	, m_sResultBlackboardKey("")
	{}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	// CBehavior
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	std::string m_sResultBlackboardKey;
};