#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CGoToBlackboardPosition : public CBehavior {
public:
	CGoToBlackboardPosition(CBehaviorTreeComponent* owner)
	: CBehavior(owner)
	, m_sBlackboardKey("")
	, mTargetPos(vmake(0.0f, 0.0f))
	, m_fArriveDistance(0.0f)
	, m_v2Offset(vmake(0.0f, 0.0f))
	, m_bKeepUpdatingPosition(false)
	{}

	// CBehavior
	virtual void init(TiXmlElement* behaviorElem) override;
	virtual void abort()                          override;

protected:
	virtual void onEnter() override;
	virtual EStatus onUpdate(float step) override;

private:
	std::string m_sBlackboardKey;
	vec2 mTargetPos;
	vec2 m_v2Offset;
	float m_fArriveDistance;
	bool m_bKeepUpdatingPosition;
};

