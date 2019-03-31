#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CChangeSpeed : public CBehavior {
public:
	CChangeSpeed(CBehaviorTreeComponent* _pOwnerComponent)
	: CBehavior(_pOwnerComponent)
	, m_fSpeed(0.0f)
	{}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;

protected:
	// CBehavior
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	float m_fSpeed;
};