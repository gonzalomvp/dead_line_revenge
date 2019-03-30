#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CCalculateRandomPosition : public CBehavior {
public:
	CCalculateRandomPosition(CBehaviorTreeComponent* owner)
	: CBehavior(owner)
	, m_sResultBlackboardKey("")
	{}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual EStatus onUpdate(float step) override;

private:
	std::string m_sResultBlackboardKey;
};

