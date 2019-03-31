#pragma once
#include "components/behavior_tree/Behavior.h"

class CBehaviorTreeComponent;

class CGoToBlackboardPosition : public CBehavior {
public:
	CGoToBlackboardPosition(CBehaviorTreeComponent* _pOwnerComponent)
	: CBehavior(_pOwnerComponent)
	, m_sBlackboardKey("")
	, m_v2TargetPos(vmake(0.0f, 0.0f))
	, m_v2Offset(vmake(0.0f, 0.0f))
	, m_fArriveDistance(0.0f)
	, m_bKeepUpdatingPosition(false)
	{}

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;
	virtual void abort()                            override;

protected:
	// CBehavior
	virtual void    onEnter ()                  override;
	virtual EStatus onUpdate(float _fDeltaTime) override;

private:
	std::string m_sBlackboardKey;
	vec2        m_v2TargetPos;
	vec2        m_v2Offset;
	float       m_fArriveDistance;
	bool        m_bKeepUpdatingPosition;
};

