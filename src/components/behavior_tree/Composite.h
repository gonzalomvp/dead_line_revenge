#pragma once
#include "components/behavior_tree/Behavior.h"

#include <vector>

class CBehaviorTreeComponent;

class CComposite : public CBehavior {
public:
	CComposite(CBehaviorTreeComponent* _pOwnerComponent) : CBehavior(_pOwnerComponent) {}
	~CComposite();

	void addChildBehavior(CBehavior* _pBehavior) { m_vChildBehaviors.push_back(_pBehavior); }

	// CBehavior
	virtual void init(TiXmlElement* _pBehaviorElem) override;
	virtual void abort()                            override;

protected:
	std::vector<CBehavior*> m_vChildBehaviors;
};