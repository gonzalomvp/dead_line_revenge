#pragma once
#include "components/behavior_tree/behavior.h"

class CBehaviorNodeTreeComponent;

class CGoToBlackboardPositionAction : public CBehaviorNode {
public:
	CGoToBlackboardPositionAction(CBehaviorNodeTreeComponent* owner)
	: CBehaviorNode(owner)
	, m_sBlackboardKey("")
	, m_pTargetEntity(nullptr)
	, m_fArriveDistance(0.0f)
	, m_bKeepFollowing(false)
	{}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;
	virtual void abort()                          override;

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	std::string m_sBlackboardKey;
	Entity* m_pTargetEntity;
	vec2 mTargetPos;
	vec2 m_v2RandomOffset;
	float m_fArriveDistance;
	bool m_bKeepFollowing;
};

