#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CRotateAim : public CBehavior {
public:
	CRotateAim(CBehaviorTreeComponent* owner) : CBehavior(owner), m_fAngle(0.0f) {}

protected:
	virtual EStatus onUpdate(float step) override;

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;

private:
	float m_fAngle;
};

