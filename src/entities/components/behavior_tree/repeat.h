#pragma once
#include "group.h"

class CBehaviorNodeTreeComponent;

class CRepeatNode : public CGroupNode {
public:
	CRepeatNode(CBehaviorNodeTreeComponent* _pOwner) : CGroupNode(_pOwner), m_iCounter(0) {}

	// CBehaviorNode
	virtual void init(TiXmlElement* behaviorElem) override;

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

	int m_iTimes;
	int m_iCounter;
};
