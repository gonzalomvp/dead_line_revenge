#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CAim : public CBehavior {
public:
	CAim(CBehaviorTreeComponent* _pOwnerComponent)
	: CBehavior(_pOwnerComponent)
	, m_sBlackboardKey("")
	{}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	// CBehavior
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	std::string m_sBlackboardKey;
};