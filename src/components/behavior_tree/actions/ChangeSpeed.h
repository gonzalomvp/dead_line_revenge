#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CChangeSpeed : public CBehavior {
public:
	CChangeSpeed(CBehaviorTreeComponent* owner) : CBehavior(owner), m_fSpeed(0.0f) {}

protected:
	virtual EStatus onUpdate(float step) override;

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

private:
	float m_fSpeed;
};

